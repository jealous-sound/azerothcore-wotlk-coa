-- Widen item_template.description for harvested Ascension item data.
--
-- The live realm's item_template holds descriptions longer than stock
-- AzerothCore's varchar(255). Of the 562,380 items harvested out of the client's
-- itemcache, 388 carry a description over that limit, the longest being 621
-- bytes. The text is real -- ability explanations and rank breakdowns such as
-- "Teaches you Rush of Adrenaline (Rank 5) on use." -- so truncating it would
-- discard server data we cannot get back once the realm is gone.
--
-- Safe against the core: ItemTemplate::Description is a std::string, populated
-- by fields[101].Get<std::string>() in ObjectMgr::LoadItemTemplates. There is no
-- fixed-size buffer anywhere in the read path, and the field is sent to the
-- client as a null-terminated string, which is how the client received these
-- lengths from the live realm in the first place.
--
-- 1024 rather than 621 leaves headroom without reaching for TEXT, which would
-- move the data off-row for no benefit at this size.

ALTER TABLE `item_template`
    MODIFY COLUMN `description` VARCHAR(1024) NOT NULL DEFAULT '';
