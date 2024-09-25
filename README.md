
# Slurm Epilogue "variety_id" (SEVI)

This repository contains a source code of a `slurmctld` epilogue that requests processing of resource utilization of a completed job.
This is a component of SLURM-LDMS project (https://github.com/algo74/slurm).

 Copyright (c) 2024 Alexander Goponenko. University of Central Florida.
 
 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 “Software”), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and
 to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


## Installation

To produce an executable, `sevi`, simply run 
```bash
make 
```


## Use

Example of use:
```bash
sevi -c /path/to/vinsnl.json
```
The command reads job id from environment variable `SLURM_JOB_ID` and "variety_id" from variable `LDMS_VARIETY_ID`.
Then, it sends requests to process finished job to an "Analytical Services" server.

The server address can be configured with the command line options (see below) or with environment variable `VINSNL_SERVER`. 
The format for the environment variable is "<_server_name_>:<_port_>".
If the server address is not specified, the default is "127.0.0.1:9999".

### Options

**-c, --config**  
path to a config JSON file (overwrites env. variable).
An example of the JSON file:
```json
{
  "server": {
    "name": "localhost",
    "port": 9999
  },
  "track_nodes": true,
  "backfill_type": null
}
```
> `sevi` reads only the `"server"` part of JSON and ignores the rest.

**-n, --name**  
name of the host (overwrites config).  

**-p, --port**  
port of the host (overwrites config).


## Tests

The test suite operates in MacOS environment only.
Modify `testsuite/Makefile` to run in another environment.
