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

<details>
  <summary><b>State</b></summary>
  
  The state controls what current state the puzzle or room is currently in. Each state has an `on` function and a `loop` function. The `on` function runs the first time the state has been changed whereas the `loop` function runs continously every loop that state is selected. There are 5 different states that exist:
    
  - 0 `Ready` - The puzzle is ready to be put into the `Active` state.
  - 1 `Active` - The puzzle currently running and can be put into the `Finished` or `Paused` state.
  - 2 `Paused` - The puzzle is currently paused and nothing can be solved unless returned to the `Active` state
  - 3 `Finished` - The puzzle has been solved/completed and is ready to be put into the `Resetting` state.
  - 4 `Resetting` - The puzzle is being reset. Conventionally this returns to the `Ready` state once it has finished resetting.

  Keep in mind that the states do not have to follow the order provided and will often need to jump between the different states as required by the Electron Escape users. The same state can also be sent to a puzzle multiple times which will trigger the `on` function every time a state has been sent.

  Any payload sent to `roomName/puzzleName/state` must constist of a variable named `state` and an integer declaring what to set the state to. An example payload that will set the state to `Ready` is:
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
  
</details>

<details>
  <summary><b>Insights</b></summary>

  The insights payload type is where puzzles are able to send data about what is currently happening (such as a button being pressed) so that Electron Escape can display the information to the user. Insights is not necessary for every puzzle but can be useful when debugging issues or without direct access to the puzzle.

  Any payload sent to `roomName/puzzleName/insights` must have atleast 1 variable and must consist of all the variables to be displayed on Electron Escape. The variables can be named anything however it must link up with the variables set on Electron Escape to be able to display the correct data. The variables can be a mixture of any regular data types.

  An example of a payload that consists of a single variable:
  ```JSON
  { "maglock": false }
  ```
  
  An example of a payload that consists of multiple variables:
  ```JSON
  {
    "switch": "on",
    "weight": 144,
    "maglock": true
  }
  ```

  A few helpful mosquitto commands to see the insights of puzzles
  - `mosquitto_sub -h 192.168.20.100 -t roomName/puzzleName/insights -v` - Display the insights for the puzzle.
  - `mosquitto_sub -h 192.168.20.100 -t roomName/+/insights -v` - Display the insights for every puzzle in a room.

</details>

<details>
  <summary><b>Set</b></summary>



</details>

<details>
  <summary><b>Command</b></summary>



</details>


<details>
  <summary><b>Logs</b></summary>



</details>

<details>
  <summary><b>Connected</b></summary>

  PuzzleClient handles the connection status of every puzzle. Whenever the puzzle connects or disconnects from the server a payload is automatically sent to `roomName/puzzleName/connected`. There are only 2 different payloads that are sent to this topic, one for when the puzzle connects and the other for when it disconnects. 

  The connection payload is sent as soon as the puzzle has connected to the server. This payload looks like:
  ```JSON
  { "connected": true }
  ```

  Once the puzzle has connected to the server, both the server and the puzzle send pings back and forth to check if the puzzle is still connected. If the server no longer detects that the puzzle is connected then it will send a disconnected payload to the same topic, which looks like:
  ```JSON
  { "connected": false }
  ```

   A few helpful mosquitto commands to see the connection status of a puzzle
  - `mosquitto_sub -h 192.168.20.100 -t roomName/puzzleName/connected -v` - Display the connection status for the puzzle.
  - `mosquitto_sub -h 192.168.20.100 -t roomName/+/connected -v` - Display the connection status for every puzzle in a room.

</details>


