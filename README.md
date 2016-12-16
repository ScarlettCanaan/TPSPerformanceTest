# TPSPerformanceTest

For the latest online version of the README.md see:
    
  https://github.com/Scarlett/TPSPerformanceTest/README.md

#About

服务器吞吐量性能测试程式

Based on libuv build a server support multithreading and multiprocess(use command line  [-t | -p] to establish multithreading or multiprocess environment). Print total received package count per 3 second.

#Installing 

You can either [Install TPSPerformanceTest via pre-built binary package](https://github.com/Scarlett/TPSPerformanceTest/bin) or [build it from source](https://github.com/ScarlettCanaan/TPSPerformance/src).

-    for source installer:

        running following command on Linux:
<pre><code>
$make server
</code></pre>
        to compile server executable file
<pre><code>
$make client
</code></pre>
        to compile client executable file
<pre><code>
$make clean
</code></pre>
        to uninstall all exexutable file


#Running Usage

-   Basic server command line usage:

    server port [[-t | -p] [count]]

For more information about the various command line options use `server -h` 

-   Basic client command line usage:

    client address port thread_count

#License

    The code in this repository is licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

      [Apache License](http://www.apache.org/licenses/LICENSE-2.0)

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

**NOTE**: This software depends on other packages that may be licensed under different open source licenses.

