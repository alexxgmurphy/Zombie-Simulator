# Zombie-Simulator
Assignment Description:

In this lab we are going to make a zombie simulation program. The idea for this is similar to
“Darwin’s World” which was developed by Nick Parlante at Stanford, and some of the design is
similar. But this lab has taken on a mind (and brains) of its own!

The basic idea is that there are zombies and humans fighting for dominance a post-apocalyptic
zombie world. Rather than hard-coding the behavior of the zombies and humans, their behavior
is off-loaded to “.zom” files. So for instance, “daryl_dixon.zom” is a pretty awesome
ranged/melee zombie killer while “michonne.zom” only does melee attacks.

On PC, the final program will look something like the image below. The Mac version will look
slightly different, but overall very similar.

There are three main aspects of this lab. The first is parsing the .zom file format and generating
the correct set of instructions from it. Once those files are parsed, then you also need the code 

that can actually execute those instructions. This is intended to take the majority of the total time
of this lab.

Hand-in-hand with the parsing is maintaining the overall simulation. For this, you will be
randomly placing zombie and humans in the world, and then running the virtual machine
simulation for every zombie and human until either the zombies or humans win. Once the
execution is working for one zombie or human, this part shouldn’t be that much more work.
Drawing will be done via a few simple wxWidgets call, similar to the last lab.
The final part is experimentation. You will make a couple of your own custom ZOM files to see
how they perform.

Before we start coding, there is one important thing to note. We are going to use exceptions.
This means you need to throw exceptions for things the user might do (such as try to pass in an
invalid .zom file) and what a ZOM simulation might do. All these exceptions also need to be
caught appropriately. If a ZOM tries to perform an invalid operation, it should just die. The
whole simulation shouldn’t come crashing down.

Part of your grade is how well you used exceptions. Make sure you create appropriate exceptions
in the Exceptions.h header file. Don’t just use std::exception everywhere. Functions that
don’t throw exceptions should be marked as noexcept (unless you are overriding a function that’s
not noexcept).
