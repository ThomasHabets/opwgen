= opwgen =

== Intro ==
opwgen generates passwords that are easy to remember, but still secure.

It takes 128 bits of random data from /dev/urandom and shows it in the
form of 12 words. There is no loss of information in converting from
128bit the input data and the words. You can convert it back again if
you want to.

I recommend 128bit (default) for crypto applications and 64bit for
login passwords. 64bit passwords are represented by 6 words.

== Installing ==
make

== Running ==

$ opwgen
lock nod watt tin more pup mont clay laws coed nibs nibs
$ opwgen
chic sear rest sang pow fin otto olga rip true guy kim
$ opwgen 2
mice gale bane egg wet mann skew sad kate dog hays tine
beck otis view fend meal andy fool mana bub leak writ gush
$ opwgen -b 64 3
feet unit lift nave sin lint
some wast are mull pry lime
lear tong dub drug laud time

== Other code ==
The opie dir contains code from libopie and has its own license. Look in
that directory for more information.
