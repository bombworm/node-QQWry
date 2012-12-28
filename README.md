# node-QQWry

A Nodejs native interface to famous IP <=> Location database qqwry.dat

## Compiling from source

    $ git clone git://github.com/bombworm/node-QQWry.git
    $ node-gyp configure build

##Usage

At first, you need to get the latest QQWry IP database from http://www.cz88.net

    var QQWry = require('./QQWry').QQWry;
    var q;
    q = new QQWry(process.cwd() + '\\' + 'qqwry.dat');
    q.QueryIP('8.8.8.8');

##Author
Zhao Lei<ace79194034@gmail.com>

##Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

##Licence
(The MIT License)

Copyright (c) 2011-2012 Zhao Lei and contributors

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.