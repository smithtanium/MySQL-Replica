# SP22-Assignment1
### Due 4/4/2022 at 11:30pm
Getting started with our application

## Getting Aquainted With the Code
In this assignment, you're expected to ensure that your application can correctly process three commands ("help", "version", "quit"). In fact, we've already implemented "quit" for you!  For many of you, completing the work necessary to get 100% of the points for this assignment will take less than an hour to complete. 

Of course, there's always a "catch". In this case, the catch is that the real work will be to get started designing and developing your database `Storage` engine for the next assignment. More on that in a moment.

Take some time this week to review the code we've provided for you. All the files are intended to serve as the basis for your own database application. 

> NOTE: While we've provided you some starting code and suggest you take advantage, you ARE NOT required to do so. You're welcome to use your own code instead. You're also welcome to make any changes you like the files we've provided. Of course, any files you add to your repository must be original work (created by you or your teammate) -- you are not permitted to use code from another source. 

### We're using C++ 17 in this class

Make sure you have an IDE (editor, debugger) that generates code according to the C++ 17 standard. Visual Studio and XCode can both do this. You're going to be expected to work with new idioms that are only available in C++ 17.

## Challenge #1 

Find the class called, `AboutUs.hpp`. For each member of your team, fill out each given field (name and github uesrname). Also indicate how many students are in your group with `getTeamSize()`.  This helps us identify students for each team.

## Challenge #2
Your first challenge is to implement the "help" command in your `Application` (command-processor). When a user types the "help" command with no arguments, your system should respond with the message: "Help system ready!".  In future versions of the app, you will provide command-specific help prompts. You're welcome to get started on that now if you so desire.

## Challenge #3
Your second challenge is to implement the "version" command in your `Application` (command-processor). When a user types the "version" command, your system should respond with the current version of your application, which is "0.1". Your output should say:  "Version 0.1". 

## Challenge #4 (ungraded)

Nows a great time to get started thinking about how you're going to handle storing database information. As was the case in ECE141a, we'll use a streams of fixed-sized blocks. Checkout the files `BlockIO.hpp` and `BlockIO.cpp` for a staring toolkit for working with block storage. You'll likely build your storage system on top of the `BlockIO` and `Block` classes. We'll discuss this in lecture during the first week. 

## Submitting your work

Check your files using git (and check the auto-grader output) provided by Vlad-the-compiler.

### Grading

```
compile test: 30pts
app test: 70pts  (runs the help, version, and quit commands)
```



