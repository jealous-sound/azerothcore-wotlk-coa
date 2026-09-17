-- #122: "Rune of Ascension" (item 375250) already carries BAG_FAMILY_MASK_CURRENCY_TOKENS (8192) in
-- `item_template`, but ObjectMgr::LoadItemTemplates strips that bit for any item missing from
-- sCurrencyTypesStore. With the bit gone, ItemTemplate::IsCurrencyToken() is false, Player::CanStoreItem
-- never routes the reward to CURRENCYTOKEN_SLOT_START..CURRENCYTOKEN_SLOT_END (118..149), so runes pile up
-- in ordinary bag slots and Player::AddKnownCurrency is never reached -- PLAYER_FIELD_KNOWN_CURRENCIES
-- stays 0 and the client's Character -> Currency page has nothing to list.
-- The server's extracted dbc/CurrencyTypes.dbc holds only the 26 stock WotLK rows, while the client's
-- DBFilesClient\CurrencyTypes.dbc (patch-M) defines ID 364 / ItemID 375250 / CategoryID 41 / BitIndex 35,
-- with CurrencyCategory.dbc naming category 41 "Ascension". The native DBC SQL loader merges
-- `currencytypes_dbc` into sCurrencyTypesStore keyed by `ItemID`, so restoring the record here needs no
-- binary DBC replacement, no core change and no client change.
-- Only `ItemID` and `BitIndex` are read by the core (CurrencyTypesfmt "xnxi"); `ID` and `CategoryID` are
-- stored for parity with the client record. `BitIndex` must stay within 1..64 because AddKnownCurrency
-- shifts 1LL << (BitIndex - 1) into the two-dword PLAYER_FIELD_KNOWN_CURRENCIES.
-- When extending this table, keep the row with the greatest `ID` also holding the greatest `ItemID`:
-- DBCDatabaseLoader::Load reads `ORDER BY ID DESC` and sizes the index table from the first row's `ItemID`
-- alone, then writes indexTable[ItemID] for every row, so an unsorted bulk import writes out of bounds.
DELETE FROM `currencytypes_dbc` WHERE `ID` = 364 OR `ItemID` = 375250;
INSERT INTO `currencytypes_dbc` (`ID`, `ItemID`, `CategoryID`, `BitIndex`) VALUES
(364, 375250, 41, 35);
