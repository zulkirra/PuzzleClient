# PuzzleClient

## Topic Design

### Puzzles
| Topic                     | Description                       | Example                       |
| ------------------------- | --------------------------------- | ----------------------------- |
| room/puzzleName/state     | Set the state for the puzzle      | { "state": 4 }                |
| room/puzzleName/insights  | Insight data for ElectronEscape   | { "insight": "value" }        |
| room/puzzleName/set       | Set variables on the puzzle       | { "variable1": 100 }          |
| room/puzzleName/command   | Run a function on the puzzle      | { "function1": [ "param1" ] } |
| room/puzzleName/logs      | MQTT debug output (without JSON)  | Hello World!                  |
| room/puzzleName/connected | Connection stnatus of the puzzle  | { "connected": false }        |
