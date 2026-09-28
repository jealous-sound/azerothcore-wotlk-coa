# mod-hero-call-board

The **Hero's Call Board** (Stormwind, `402000`) and the **Warchief's Command Board** (Orgrimmar, `402001`) with
47 daily quests, answered in the form the Ascension client's Call Board window expects:

- The board opens on its three categories (**PvE**, **Profession**, **PvP**) and sends the daily, weekly and arena
  reset times the window's timers read. It always shows the yellow "available" marker.
- A category lists the quests in progress and the quests the character may take, filtered before the 32-entry menu
  limit, and sends their quest data ahead of the menu.
- Accepting (including **Accept All**), refusing, turning in or abandoning a board quest re-sends the open category
  instead of closing the window. Every quest still passes the core's own checks.
- Dailies that need a server event are credited here: five duel victories (`80651`), a battleground win (`81260`),
  and dungeon finder clears of a Normal, Heroic or Mythic dungeon (`81042`, `81041`, `80652`).

The board rewards are the quests' own Runes of Ascension. The Callboard Cache is handled by the CoA core.

## Configuration

`conf/hero_call_board.conf.dist`: `HeroCallBoard.Enable` (default `1`).
