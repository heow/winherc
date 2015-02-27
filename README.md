# WINHERC

## Historical Software - Windows 3.0 CGA to HGC Clipboard Copying Program

### 1980s

The time is the late 80s, I'm not going to go on about how bad it was or how fast things are now.  I'm going to talk about how much it ROCKED.  

You see, I had a Hercules Graphics Card (HGC/Herc).  All your text was beautifully rendered in amber at an astounding 720x348 resolution:

![leading edge hotlink](http://upload.wikimedia.org/wikipedia/commons/thumb/d/db/LeadingEdge_Front_with_keyboard.JPG/450px-LeadingEdge_Front_with_keyboard.JPG)

I really enjoyed it, the high resolution was much easier on the eyes than the C64 (or Z80), plus you could push it into graphics mode and have plenty of bits.

The only downside was that it was a single color, but hey when you're looking at it all day who cares?

My friend Bill scored a Color Graphics Adapter (CGA).  Sure color was nice, but the resolution was relatively low and HGC held up nicely with almost double the resolution.

Of course it was Bill's idea to plug the CGA and Herc into [Herbie Junior, his Leading Edge Model D](http://en.wikipedia.org/wiki/Leading_Edge_Model_D) at the same time and see what happens.  The result was underwhelming, thankfully it didn't fry our 8088s and the machine booted using CGA, ignoring Herc.

Booting between the two, I realized that the Herc "remembered" the text on the screen when the CGA was in operation.  That means it could be addressed directly!

At that point I don't know exactly how I figured the Herc's direct addresses but it wasn't that hard.  I wrote a C program to write "X"s at memory addresses while watching the Herc.  

Remember if you were lucky you had 640kb so it wasn't long before finding out it lived at memory address <pre>B000</pre>

### Version 1.0 CGA2HGC TSR

Living in DOS you don't have many things, among them was multitasking.  This could be faked out by having your program permanently live in memory to be executed by an interrupt, say a key-combination.  It's called Terminate and Stay Resident (TSR), meaning you can be running any program (dBase2 or WordStar) and ALSO run your program.

The first TSR I wrote copied the CGA text to my Herc, allowing you to use TWO MONITORS at the same time!   You could be looking at source (say a header file or dBase table), hit a few keys, copy the text over to the second monitor as a reference, then back to the code.

How could life ever get better?

### Fenestration

Bill and I had been using Windows since 2.x  It was ok, but 3.0 was a big departure and actually useable.  However graphics was a new realm rendering my text-based TSR useless.

With my extra Herc card and amber monitor, what's a boy to do?  Grab a copy of Petzold's _"Programming Windows"_, fire up your Borland compiler and get to work writing a Windows version.

### Version 2.0 WINHERC

When running Windows 3.0, WINHERC allows you to copy text to your second monitor connected to a Hercules Graphics Card.  Any text copied to the clipboard is automatically displayed on the second monitor, with blank lines removed to maximize space.

Nice.

The program is pretty short, if you compare the source probably 80% of the boilerplate is Petzold's _Hello World_.  :-)

I used this software daily for a long time.  It's been  uploaded it to BBSes as well as the FTP sites on the nascent Internet.

Sad to say it stopped working when I got my first 386sx, I don't know what was to blame: motherboard, chipset or my old card but plugging an HGC into the 386sx with another graphics card caused random characters to be displayed.  You also couldn't overwrite them.
