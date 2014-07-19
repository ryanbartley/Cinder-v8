This is a light Cinder Block exposing Chrome's V8 script parsing technology to Cinder, created by myself and [@galsasson](https://github.com/galsasson).

There are four samples that show a bit of the backbone of V8. The first three are simple, pulled from the V8 website and put into Cinder. The fourth example shows how to wrap Vec3f for use inside javascript. 

Samples.
- The first is a simple HelloWorld showing the creation of a context and parsing a "Hello, World!" string.
- The second sample opens a Javascript shell and processes the information.
- The third sample shows the beginning of wrapping objects. I used this to create the fourth sample.
- The fourth sample shows how you can wrap Cinder objects or your own objects and use them in Javascript.

Build instructions are on this [page](https://code.google.com/p/v8/wiki/BuildingWithGYP)

For Macs, here's the abridged version...

First, you need GYP itself. On systems that have make (Linux, Mac, most other Unixes), this is as easy as:

	make builddeps

Now you can either make v8 using cMake or use gyps. I use gyps because it's faster...

From the root of your V8 checkout, run either of:

	build/gyp_v8 -Dtarget_arch=ia32
	build/gyp_v8 -Dtarget_arch=x64

This will generate Xcode project files in build/ that you can then either open with Xcode or compile directly from the command line. For instance, either of these:

	xcodebuild -project build/all.xcodeproj -configuration Release
	xcodebuild -project build/all.xcodeproj
	
That should create fat libraries needed to run with v8. if you have any problems, take a look at the full pages. Next You'll need to go to the Cinder-v8 directory and create a lib directory that has a macosx folder and a folder for which configuration you used to build( Debug folder for Debug build, Release folder for Release build or both) and put these fat files inside that folder like this...

	Cinder-v8 /
	--- lib /
	------ macosx /
	--------- debug / (or release)
	------------ libicudata.a
	------------ libicui18n.a
	------------ libicuuc.a
	------------ libv8_base.a
	------------ libv8_nosnapshot.a
	------------ libv8_snapshot.a

Take a look at https://developers.google.com/v8/intro for more info. Soon I'll put everything up on line, it's just too big for github.

Let me know if there are any problems.
