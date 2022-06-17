# SP22-Assignment2
### Due Monday, April 11, 11:30pm
Database Command Processing

## Managing Database (Storage) Files

In this assignment, your team will implement "Database Level" commands that relate to creating, listing, and managing database containers. For example, a user of your application might issue the following command:

```
CREATE DATABASE Test1; 
```

This will result in a new database container named `Test1` being created in a prescribed folder on your harddrive. 

> **NOTE:** Command keywords are not case senstive, we just write keywords in ALL-CAPS for emphasis.

### Getting Started

To get started, check out this assignment from github. This assignment builds on assignment #1.  If you created any _new_ files for assignment #1, please copy them into the project folder for this assignment. 

> **BE CAREFUL** -- Do not overwrite any of the new files in this assignment.    

## New and Updated Files...

In this version of the assignment, we've added a few new files for your use.  You are NOT required to use these classes, but they will be helpful in delivering the functionaliy that is expected.  They are outlined below:

### `Database.hpp/.cpp` 

These files contain a minimal starting point for your `Database` class. This class will have a significant role in future assignments. It will implement database logic to add, update, delete rows, and much more. For now, it's mostly a placeholder where you can get started.

### `DBProcessor.hpp/.cpp` 

These files contain a minimal starting point for your `DBProcessor` class. This class will participate as a command-processor for database-related commands. 

### `FolderReader.hpp` 

This class is designed to help you read the contents of a given folder. It provides a visitor method called "each", whereby a delegate can get called back with each of the files found in the given folder. Perfect for use with `FolderView` below.

### `FolderView.hpp` 

This file is designed as the starting point for a view you can use when the user issues the `SHOW DATABASES` command. Please watch this week's video for suggestions on how to implement this view.  You may use this file, or ignore if you have a different way you prefer to implement this.

### `Storage.hpp/.cpp` 

The `Storage` class is used for high-level data input/output in your application. In assignment #1, we provided you with a `BlockIO` class that handles reading/writing individual `Block` objects. The `Storage` class is designed to give you "stream-oriented IO".  We will discuss this class during lecture - to help you make sense of how to use it.  Although we provided the `Storage` class declartion too you now, you really don't need it until next week.  We provided this to you early so you could start thinking about how you save "streams" of data into a database file. 

You're welcome to use this design, or ignore this class and develop your own.  These classes are only here to help you get started.

#### Understanding the Storage Class

> **NOTE:** We're going to cover over this concept in lecture soon. But here's an introduction to the idea. Skip for now if you prefer.

In assignment 1.0, we provided a class called `BasicIO` which can handle reading and writing `Block` objects, like `Row` and `Entity` objects that are guaranteed to fit into a `Block`. Storage blocks are great for `Row` and `Entity` objects because we want a very fast way to find, read, and write these objects. 

There are other cases where we have data that doesn't fit neatly into a single block. For example, imagine the case where we have an `Index` where each key/value pair occupies 8 bytes of memory. If we have a table of 1000 rows, the primary-key index for our table will require 8K bytes. Using a "block-oriented" API to read/write an `Index` would be very inconvenient. Instead, we'd like to use a standard stream interface for reading/writing our index.

We've provided you a starting point for a new class called `Storage`. The `Storage` class ia sub-class of `BlockIO`, that provides an API to read/write data streams. This was designed to handle things like our `Index` class. Here's how you might use it to save your `Index` (of 1000 key/value pairs) to a database file.

1. Write your `Index` into a regular stream. We like using `stringstream` for this purpose.
2. Call `Storage::save(std::iostream &aStream, StorageInfo &anInfo)`, passing the buffer you created from step 1

Your `Storage:save()` method should determine how big the given stream is, and calculate how many 1K `Blocks` it would need to store that much data.  Next, it will find (or create) that many blocks. Finally, it will copy data from the given stream, one block at a time, and save it into the database file. 

You'll want to find a way to create logical "associations" between blocks that belong to a stream.  In our implementation we use extra data properties in the `Block::header` to indicate which stream the block belongs to, the order of the block in the sequence, and so forth.

Again, we'll cover this concept more in class. For now, we're just trying to get you to think about storing "streams" of data in your database.


### A New Database Command Processor 

This week in Assignment#2 we're focusing on "Database level" commands (see the list below). Handling and processing of these commands is very different than handling the "Application Level" commands from your first assignment, such as `version` and `quit`. 

We **strongly** suggest your create a new subclass of the `CmdProcessor` class to handle these "Database Level" commands. In our reference implementation, we called our `DBProcessor` (but you can do whatever you like).  

When the user issues a "Database Level" command that your `Application` class doesn't understand, it should forward theses requests to this new "Database Level" command processor. You might choose to use "Chain-of-responsibilty" design pattern for this purpose.

<hr> 

## New Commands (The 5 commands for this week)

There are five new commands to be implemented in this assignment:

```
1.  CREATE DATABASE {db-name}
2.  DROP DATABASE {db-name}
3.  SHOW DATABASES
4.  USE {db-name}
5.  DUMP DATABASE {db-name}
```

The actions your application will take for each of theses command will be the same as what MySQL does. Let's discuss each of them.

#### Challenge #1:  Implement your `FolderReader` class

The `FolderReader` class is designed to read a set of files in a given folder, and call a visitor with each name.  This class will be used by the `FolderView` class, which should be used when someone enters a `SHOW DATABASES` command.

The test for this assignment require that your `FolderReader` class is working. During the test, the auto-test will create a `FolderReader` and ask it to visit each file with the ".txt" extension within the default storage folder.  There should be at least one (the test will generate it).

> **NOTE:** Remember that the "version number" of your `Application` should be incremented each week. This week, your `Application` should report that it is running "Version 0.2". 

#### Challenge #2: `CREATE DATABASE {db-name}`  

This command results in a new database (storage file with .db extension) being created in the prescribed storage location. All of your databases will be contained with a "storage" folder that you define. See the reference to the `Config.hpp` file above for more information.

When you create a new database storage file, it should contain at least one block of data, and thus a filesize of 1K. We recommend that you use the first block (blocknumber=0) to hold meta information about your database. What you store here (and how you encode it) is up to you. In our reference implementation, we store our "entity index" (key=name, value=blocknum) in block 0.

After this commmand runs, the output you write to the `output` stream should look as shown below. You're expected to create a custom `View` class to produce this output:

```
CREATE DATABASE testing1;
Query OK, 1 row affected (0.01 secs)  
```

> NOTE: Use the provided `Timer.hpp` class to compute and output the elapsed time, that the `CREATE` command took to complete.

#### Challenge #3: `DROP DATABASE {db-name}`  

When the user issues the `DROP` command, she is trying to deleted a known database container.  If the database being dropped is also the active database (loaded into memory), it should be removed.  At this point, no database is loaded, and table related commands would be ignored. Here's the sample output from this command (again, you're expected to create a custom `View` class to produce this output):

```
DROP DATABASE {db-name}
Query OK, 0 rows affected (0.00) 
```

Your code should validate that the database file exists, and if so, remove that file from the default folder where you keep database files. Since we're using C++ 17, you can use the `std::remove` function to delete the file (and this works in a cross-platform way):

```
std::remove("full-path-to-db-file");
```

#### Challenge #4: `SHOW DATABASES`  

This command is interesting, because it has no impact on the database in use (in memory). The `SHOW DATABASES` command needs to show your user a list of the databases that have been created so far (one per line). You're expected to create a custom `View` class to produce this output:

```
> show databases;
+--------------------+
| Database           |
+--------------------+
| foo                |
| bar                |
| wiltz              |
+--------------------+
3 rows in set (0.02 sec)
```

To do this, you must iterate the files in the prescribed storage location (specified in `Config.hpp ::getStoragePath()`). You need to use a library that can iterate files in a folder/directory. Fortunately, in C++ 17, we have such a library:

```
#include <filesystem>
```

The `<filesystem>` library provides platform independent function calls that let you do all kinds of different operations on files and folders on your local harddrive:

1. Iterate files in a directory/folder
2. Get path information
3. Manipulate files (copy, move, create, link...)
4. Change file permissions
5. Get size and meta data about a file
6. *and more*

Please be sure to watch this weeks video about ["Closures and Views"](https://youtu.be/yX_qRSi6huU). It can really help you with this task.

#### Challenge #5: `USE {db-name}`  

The `USE {db-name}` command makes a known database available in your application.  Generally, this means confirming the storage file exists, opening the file, and getting your application ready to process subsequent commands related to the chosen database. While you're free to implement your database any way you like, in our reference implementation - when the `use DB` statement is issued -- we create a `Database` object, initialize it to open the associated file, and we preload whatever `Entity` objects have been created in the database.  (We won't create any `Entity's` until next week). 

Here's an example of the expected output when you issue the `USE {db-name}` command:

```
> use testing;
Database changed
```

#### Challenge #6: `DUMP DATABASE {db-name}`  

This command does not exist in MySQL -- we've created it specially just for debugging purposes.  The `DUMP DATABASE {db-name}` command should iterate through every block in the associated database storage file, and display descriptive information about each block. We have found that this command can be very helpful while you're debugging your database application.  Here is the sample output (again you're expected to create a custom `View` class to produce this output):

```
> dump database testdb;
+----------------+--------+---------------+
| Type           | Id     | Extra         |
+----------------+--------+---------------+
| Meta           | 0      |               |
-----------------+--------+---------------+
| Entity         | 1      | Users         |
-----------------+--------+---------------+
| Data           | 1      | Users         |
-----------------+--------+---------------+
| Free           | 0      |               |
-----------------+--------+---------------+
| Data           | 3      | Users         |
-----------------+--------+---------------+
5 rows in set (0.02 sec)
```

**NOTE:** You are free to output any information you like for each block. However, the first column MUST refer to the block type, and describe the block according to the types you have defined. By default, we've defined block types of "Meta", "Entity", "Data", "Index", and "Free". 

A `BlockType enum` has been declared for you in the `BlockIO.hpp` file. You may change this enum list to anything you like, or replace it altogether.  

For example, in our example we appended two columns (Id and Extra). Our database implementation uses this information to track/link related data elements. You are not required to use them, and can ignore the following explanation if you don't intend to do so.

For the Id column, when our block is an `Entity` type, the Id represents the hash value of that `Entity`. If the block is `Data` type, `Id` will refer to the primary key value of that `Row`. This helps us quickly idetify the row without parsing the payload from the block. Again, you don't have to use this methodology if you don't want to do so.

Again, in your output, you MUST include the `Type` column but are free to append additional columns that help your debugging.

<hr> 

## A Note About Console Output

### Logging 

In order for the auto-grader to work correctly, it captures all your logging output to an internal stream for validation. When your code runs commands, it is often expected to send output of a command to the terminal. To do so, use the `output` stream declared in your `CmdProcessor`. This is provided for you (and is available to every subclass of `CmdProcessor`, including the `Application` class. 

### Debugging 

For debugging purposes, if you want to send output directly to the terminal (not logged), you can write to the `std::cerr` or `std::cout`. 

<hr> 

## Submitting Your Work (Due Monday, April 11, 11:30pm)

As always, you will submit your work by pushing changes to your team repository.  You may do so as many times as you wish before the assignment is due. Each time you submit, our auto-grader will test your code according to the 5 challenges. 

> **NOTE:** DON'T FORGET to include your "AboutUs" class -- we use this class to determine who will get credit for the assignment. 
 
### Grading 

```
1. Compiles:  5% 
2. App Test:  10% (from last week)
3. Reader Test:   10%
4. DBCommands Test: (Create, use, show drop, dupm): 75%
```

Good luck!


