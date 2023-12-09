# Somnos

Setup build environment:
```bash
./project.py submodules
./project.py setup
```

Build the project:
```bash
./project.py build
```

## Functional requirements
- switching USB power on/off
  - at a certain time of day
  - when requested
- measuring and reporting USB current/power
  - buffer measured data from some period of time
  - report data with timestamp when requested

## Nonfunctional requirements
- communication over ethernet with TCP
- time obtained through SNTP

## To do
- finish communication API - add class
- add SNTP support
- main application
  - on startup get current time, turn power on/off based on it
  - use ring buffer to store data over time
- final cleanup
