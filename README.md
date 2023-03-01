# PuzzleClient #

## Topic Design Tree ##
### Base Topics ###
All puzzles that use the PuzzleClient use a specific topic heirachy. In order for the puzzle to be compatible with Electron Escape it **MUST** follow this heirachy otherwise it will not display the right data.

All topics follow a convention which starts with the room name, then the puzzle name, then finally the payload type. For example:
``` Text
roomName/puzzleName/payloadType
```
An Exception to this convention is for the state of the room itself. This can be accessed through:
``` Text
roomName/state
```

### Payload Types ###
There are a total of 6 different payload types that are compatible with Electron Escape.
- [State](#State) - e.g. `roomName/puzzleName/state` or `roomName/state`
- [Insights](#Insights) - e.g. `roomName/puzzleName/insights`
- [Set](#Set) - e.g. `roomName/puzzleName/set`
- [Command](#Command) - e.g. `roomName/puzzleName/command`
- [Logs](#Logs) - e.g. `roomName/puzzleName/logs`
- [Connected](#Connected)- e.g. `roomName/puzzleName/connected`

#### State ####
The state controls what current state the puzzle or room is currently in. Each state has an `on` function and a `loop` function. The `on` function runs the first time the state has been changed whereas the `loop` function runs continously every loop while that state is selected. There are 5 different states that exist to cover most functions:
- 0 `Ready` - The puzzle is ready to be put into the `Active` state.
- 1 `Active` - The puzzle currently running and can be put into the `Finished` or `Paused` state.
- 2 `Paused` - The puzzle is currently paused and nothing can be solved unless returned to the `Active` state
- 3 `Finished` - The puzzle has been solved/completed and is ready to be put into the `Resetting` state.
- 4 `Resetting` - The puzzle is being reset. Conventionally this returns to the `Ready` state once it has finished resetting.

Keep in mind that the states do not have to follow the order provided and will often need to jump between the different states as required by the Electron Escape users. The same state can also be sent to a puzzle multiple times which will trigger the `on` function every time a state has been sent.

Any payload sent to the state topic must constist of a variable named `state` and an integer declaring what state to be set to. An example payload that will set the state to `Ready`:
```JSON
{ "state": 0 }
```

*Similar to puzzles, the state of the room can also be changed by sending the payload to `roomName/state` instead.*

A few helpful mosquitto commands to manage the state of a puzzle or room:
- `mosquitto_sub -h 192.168.20.100 -t roomName/puzzleName/state -v` - Display the state of the puzzle.
- `mosquitto_sub -h 192.168.20.100 -t roomName/+/state -v` - Display the state for every puzzle in the room.
- `mosquitto_sub -h 192.168.20.100 -t roomName/state -v` - Display the state of the room.
- `mosquitto_pub -h 192.168.20.100 -t roomName/puzzleName/state -m "{\"state\":0}"` - Set the state of the puzzle (in this case to `Ready`).
- `mosquitto_pub -h 192.168.20.100 -t roomName/state -m "{\"state\":0}"` - Set the state of the room (in this case to `Ready`).

#### Insights ####
Insights helps Electron Escape to be able to display what is happening with that particular device

#### Set ####

#### Command ####

#### Logs ####

#### Connected ####
