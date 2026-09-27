Ascension client item patches: use sparingly. These packets replace client memory
records without distributing a client update. They do not change server item
templates or files on disk. Patched properties can differ from both the original
DBC and server data, making the true properties difficult to inspect and debug.
Preserve complete records and restrict patches to deliberate presentation fixes.
Shared display, cost and set rows can affect multiple items.

Layouts verified against Extensions.dll SHA256:
0f8d847b3adc44a963606f0cd4f7938ad6fcd6f4d87feac3131c7153bdf3bb11.
See item_layouts.json for handler RVAs, numeric counts and pointer placeholders.
Each packet is one record: uint32 words, then uint32-length-prefixed byte strings.
There is no record count or string terminator. Pointer placeholders are zeroed;
strings are actual text, never DBC offsets or client process addresses.
SMSG_PATCH_ITEM patches the eight-word Item.dbc row, not a full item template.
SMSG_PATCH_ITEM_STAT is named in the DLL, but no handler or wire layout has been
verified. Encode rejects it; its presence in the name table is not support.
No item patches are sent automatically. Reapply desired patches for each client
session; a client restart discards them. Send success is not client acknowledgement.

Inactive examples below require complete preserved fields and strings in wire
order, with only intended values changed. Consult the manifest before use.
SMSG_PATCH_ITEM_ADDON (0x0569)
Send(player, Table::Addon, addonFields, addonStrings);
SMSG_PATCH_ITEM_DISPLAY_INFO_COLLECTIONS (0x056A)
Send(player, Table::DisplayInfoCollections, displayInfoCollectionsFields, displayInfoCollectionsStrings);
SMSG_PATCH_ITEM_STAT (0x059F)
Encode(Table::Stat, {}); // Rejected until a handler and layout are verified.
SMSG_PATCH_ITEM_APPEARANCES (0x0693)
Send(player, Table::Appearances, appearancesFields);
SMSG_PATCH_ITEM_SET_APPEARANCES (0x06EC)
Send(player, Table::SetAppearances, setAppearancesFields);
SMSG_PATCH_ITEM (0x0932)
Send(player, Table::Item, itemFields);
SMSG_PATCH_ITEM_EXTENDED_COST (0x0944)
Send(player, Table::ExtendedCost, extendedCostFields);
SMSG_PATCH_ITEM_VISUALS (0x0964)
Send(player, Table::Visuals, visualsFields);
SMSG_PATCH_ITEM_VISUAL_EFFECTS (0x0965)
Send(player, Table::VisualEffects, visualEffectsFields, visualEffectsStrings);
SMSG_PATCH_ITEM_SET (0x0966)
Send(player, Table::Set, setFields, setStrings);
SMSG_PATCH_ITEM_RANDOM_SUFFIX (0x0967)
Send(player, Table::RandomSuffix, randomSuffixFields, randomSuffixStrings);
SMSG_PATCH_ITEM_RANDOM_PROPERTIES (0x0968)
Send(player, Table::RandomProperties, randomPropertiesFields, randomPropertiesStrings);
SMSG_PATCH_ITEM_PURCHASE_GROUP (0x0969)
Send(player, Table::PurchaseGroup, purchaseGroupFields, purchaseGroupStrings);
SMSG_PATCH_ITEM_LIMIT_CATEGORY (0x096A)
Send(player, Table::LimitCategory, limitCategoryFields, limitCategoryStrings);
SMSG_PATCH_ITEM_DISPLAY_INFO (0x096B)
Send(player, Table::DisplayInfo, displayInfoFields, displayInfoStrings);
SMSG_PATCH_ITEM_COND_EXT_COSTS (0x096C)
Send(player, Table::CondExtCosts, condExtCostsFields);
