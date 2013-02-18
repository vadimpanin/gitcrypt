gitcrypt
========

Allows to transparently encrypt repository contents with AES 256. The main advantage is that no additional software needed to be installed in untrusted location. Files are automatically encrypted on the fly when doing git push from trusted location.

tcrypt v0.01
Allows to transparently encrypt repository contents with AES 256 using smudge, clean and diff filters.

The main advantage is that no additional software (except git) should be installed in
untrusted location. Files are automatically encrypted on the fly when doing git push
from trusted location.

Full conversion chain is: source -> line delimiter -> Zlib -> AES 256 -> Base64.
Filename encryption is not supported.

Building
========

You'll need the following dependencies:
- boost (http://boost.org)
- zlib (http://zlib.net)
- openssl (http://openssl.org)

	apt-get libboost-dev libz-dev libssl-dev
	make

Please note: gcc 4.3 and boost <1.40 can be not quite compatible.

Installing
==========

Copy gitcrypt to any directory you have in PATH (/usr/local/bin by default).

	make install

Using
=====

You'll need a key to encrypt your files. Key may be up to 32 characters, please choose it carefully.

Creating new encrypted repository:

1. Create new git repository in untrusted location (ex. your VDS/VPS server).
2. Clone it to trusted location (your laptop).
3. Add next lines to .git/config of your cloned repository (or just run `gitcrypt init`):

    [filter "gitcrypt"]
        smudge = gitcrypt decrypt --key <KEY>
        clean = gitcrypt encrypt --key <KEY>
    [diff "gitcrypt"]
        textconv = gitcrypt decrypt --key <KEY> --out - --in

4. Create if necessary and add next line to .git/info/attributes:
   (skip this step if you ran `gitcrypt init` on the prevoius step)

<pre>* filter=gitcrypt diff=gitcrypt</pre>

Now you can add files to your repository. Work with the repository as usual.
gitcrypt will automatically encrypt the files on-fly before transferring them to untrusted location (when doing git push).

Cloning existing encrypted repository:

1. Clone existing encrypted repository to trusted location.
2. Modify .git/config and .git/info/attributes of cloned repository as described above (or just run `gitcrypt init`)
3. Run `git reset --hard` in order to decrypt files.

Work with the repository as usual.
gitcrypt will automatically encrypt the files on-fly before transferring them to untrusted location (when doing git push).


