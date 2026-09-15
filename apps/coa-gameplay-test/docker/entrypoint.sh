#!/usr/bin/env bash
# Entry point of the ac-gameplay-test Compose service.
set -euo pipefail

runner=/azerothcore-src/apps/coa-gameplay-test/run.py

if [[ "${1:-}" != run ]]; then
  exec python3 "$runner" "$@"
fi
shift

: "${MYSQL_ROOT_PASSWORD:?MYSQL_ROOT_PASSWORD must be set}"
if [[ ! -w /results ]]; then
  echo "ERROR: /results is not writable; create .cache/coa-gameplay-tests as your user first" >&2
  exit 1
fi

# The container file system is discarded with the container (--rm).
secrets="$(mktemp -d)"
escaped="${MYSQL_ROOT_PASSWORD//\\/\\\\}"
(
  umask 077
  printf '[client]\nhost=127.0.0.1\nport=3306\nuser=root\npassword="%s"\n' "$escaped" \
    > "$secrets/admin-client.ini"
)
unset MYSQL_ROOT_PASSWORD escaped

exec python3 "$runner" run "$@" \
  --worldserver /azerothcore/env/dist/bin/worldserver \
  --config /coa/source-etc/worldserver.conf \
  --modules-config-dir /coa/source-etc/modules \
  --server-modules-dir /azerothcore/env/dist/etc/modules \
  --mysql /usr/bin/mysql \
  --mysqldump /usr/bin/mysqldump \
  --database-client-config "$secrets/admin-client.ini" \
  --output "/results/$(date -u +%Y%m%dT%H%M%SZ)"
