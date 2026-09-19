import contextlib
import io
import os
import sys
import unittest
from unittest.mock import patch

import label_issues


class LabelIssuesTests(unittest.TestCase):
    def setUp(self):
        self.output = io.StringIO()
        self.stack = contextlib.ExitStack()
        self.addCleanup(self.stack.close)
        self.stack.enter_context(contextlib.redirect_stdout(self.output))
        self.stack.enter_context(contextlib.redirect_stderr(self.output))
        self.stack.enter_context(patch.object(label_issues, "REPO", "owner/repo"))

    def test_classes_categories_and_null_body(self):
        labels = label_issues.determine_labels({"title": "Pyromancer C++ damage bug", "body": None})
        self.assertEqual(labels, ["Pyromancer", "CPP Edit", "Bug"])

    def test_cpp_boundaries(self):
        for text in ("C++", "C++ change required", "Fix requires C++.", "CPP", "C++ edit"):
            with self.subTest(text=text):
                self.assertIn("CPP Edit", label_issues.determine_labels({"title": text}))
        self.assertNotIn("CPP Edit", label_issues.determine_labels({"title": "abc++"}))

    def test_no_status_from_prose_or_unchecked_boxes(self):
        for body in (
            "This has never been tested.", "Not tested", "Tested on a local server",
            "Needs to be tested", "- [ ] Tested", "- [ ] Not Tested", "I can’t test this.",
        ):
            with self.subTest(body=body):
                self.assertIsNone(label_issues.determine_testing_status(body))

    def test_explicit_status_formats(self):
        for body, expected in (
            ("Testing status: Tested", "Tested"),
            ("testing status: not tested", "Not Tested"),
            ("- [X] Tested\n- [ ] Not Tested", "Tested"),
            ("- [ ] Tested\n- [x] Not Tested", "Not Tested"),
            ("### Testing status\n\nTested\n\nMore details", "Tested"),
            ("### Testing status\n\nNot Tested", "Not Tested"),
        ):
            with self.subTest(body=body):
                self.assertEqual(label_issues.determine_testing_status(body), expected)

    def test_conflicting_statuses_are_not_guessed(self):
        self.assertIsNone(label_issues.determine_testing_status("- [x] Tested\n- [x] Not Tested"))

    def test_status_must_immediately_follow_heading(self):
        self.assertIsNone(label_issues.determine_testing_status("### Testing status\nUnknown\nTested"))

    def test_status_is_read_from_body_only(self):
        self.assertEqual(label_issues.determine_labels({"title": "Testing status: Tested"}), [])

    def test_existing_labels_are_preserved(self):
        issue = {"labels": [{"name": name} for name in ("Tested", "DB", "human-label")]}
        with patch.object(label_issues, "gh") as gh:
            label_issues.update_issue("42", issue, ["Bug", "CPP Edit", "Not Tested"])
        gh.assert_called_once_with(
            "issue", "edit", "42", "--add-label", "Bug,CPP Edit", "--repo", "owner/repo",
        )

    def test_existing_not_tested_is_not_overridden(self):
        with patch.object(label_issues, "gh") as gh:
            label_issues.update_issue("42", {"labels": [{"name": "Not Tested"}]}, ["Tested"])
        gh.assert_not_called()

    def test_repeated_run_and_case_variants_do_not_write(self):
        with patch.object(label_issues, "gh") as gh:
            label_issues.update_issue("42", {"labels": [{"name": "bug"}]}, ["Bug"])
        gh.assert_not_called()

    def test_dry_run_prints_additions_without_writing(self):
        with patch.object(label_issues, "gh") as gh:
            label_issues.update_issue("42", {"labels": []}, ["Bug", "Tested"], dry_run=True)
        gh.assert_not_called()
        self.assertIn("would add: Bug, Tested", self.output.getvalue())

    def test_pull_requests_are_skipped(self):
        with patch.object(label_issues, "get_issue", return_value={"pull_request": {"url": "fixture"}}):
            with patch.object(label_issues, "update_issue") as update:
                label_issues.label_issue("42")
        update.assert_not_called()

    def test_main_dry_run_reads_but_does_not_edit(self):
        with patch.dict(os.environ, {"ISSUE_NUMBER": "42", "DRY_RUN": "true"}):
            with patch.object(sys, "argv", ["label_issues.py"]):
                with patch.object(label_issues, "gh", return_value='{"title": "Bug", "labels": []}') as gh:
                    label_issues.main()
        gh.assert_called_once_with("api", "repos/owner/repo/issues/42")

    def test_invalid_issue_numbers_fail_before_github(self):
        for number in ("", "0", "-1", "42;echo unsafe", "https://example.com", "1.5"):
            with self.subTest(number=number), patch.dict(os.environ, {"ISSUE_NUMBER": number}):
                with patch.object(sys, "argv", ["label_issues.py"]), patch.object(label_issues, "gh") as gh:
                    with self.assertRaises(SystemExit) as error:
                        label_issues.main()
                self.assertEqual(error.exception.code, 2)
                gh.assert_not_called()

    def test_bulk_run_paginates_and_passes_preview_mode(self):
        with patch.dict(os.environ, {"ISSUE_NUMBER": "", "DRY_RUN": "false"}):
            with patch.object(sys, "argv", ["label_issues.py", "--all", "--dry-run"]):
                with patch.object(label_issues, "gh", return_value="42\n43") as gh:
                    with patch.object(label_issues, "label_issue") as label:
                        label_issues.main()
        gh.assert_called_once_with(
            "api", "--paginate", "repos/owner/repo/issues?state=all&per_page=100",
            "--jq", ".[] | select(.pull_request == null) | .number",
        )
        self.assertEqual([call.args for call in label.call_args_list], [("42", True), ("43", True)])

    def test_github_failure_stops_the_run(self):
        with patch.object(label_issues.subprocess, "run") as run:
            run.return_value.returncode = 1
            run.return_value.stderr = "API failure"
            with self.assertRaises(SystemExit) as error:
                label_issues.gh("api", "repos/owner/repo/issues/42")
        self.assertEqual(error.exception.code, 1)


if __name__ == "__main__":
    unittest.main()
