# c-environment

Config for vscode projects

.vscode <br>
├── c_cpp_properties.json <br>
├── launch.json <br>
├── settings.json <br>
└── tasks.json

OpenMP library paths:
`brew info libomp`

Example:
```c
"-I/usr/local/opt/libomp/include",
"-L/usr/local/opt/libomp/lib",
```
