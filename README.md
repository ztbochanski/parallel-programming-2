# c-environment

Config for vscode projects

.vscode
├── c_cpp_properties.json
├── launch.json
├── settings.json
└── tasks.json

OpenMP library paths:
`brew info libomp`

Example:
```c
"-I/usr/local/opt/libomp/include",
"-L/usr/local/opt/libomp/lib",
```
