<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://i.imgur.com/6wj0hh6.jpg" alt="Project logo"></a>
</p>

<h3 align="center">MySQL Replica </h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/kylelobo/The-Documentation-Compendium.svg)](https://github.com/kylelobo/The-Documentation-Compendium/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/kylelobo/The-Documentation-Compendium.svg)](https://github.com/kylelobo/The-Documentation-Compendium/pulls)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="center"> 
    The final build of a MySQL replica from a group class project in ECE 141B at UCSD. The initial commit is the final state at the end of 10 
weeks. Any changes after initial commit are mine alone. 
</p>

## 📝 Table of Contents
- [About](#about)
- [Getting Started](#getting_started)
- [Deployment](#deployment)
- [Usage](#usage)
- [Built Using](#built_using)
- [TODO](../TODO.md)
- [Contributing](../CONTRIBUTING.md)
- [Authors](#authors)
- [Acknowledgments](#acknowledgement)

## 🧐 About <a name = "about"></a>
This project was to apply the design patterns that we learned form ECE 140A. This covered patterns such as singleton,
factory, visitor, and more. This replica of MySQL performs common SQL commands such as create (DB and table), Drop(DB and table),
insert, select, update, join, and delete. 

## 🏁 Getting Started <a name = "getting_started"></a>
This program is written with C++ 17. Use the included make file to compile the program. 

### Prerequisites
None
<!--What things you need to install the software and how to install them.

```
Give examples
```
-->
### Installing
None
<!--A step by step series of examples that tell you how to get a development env running.

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo.
-->
## 🔧 Running the tests <a name = "tests"></a>
a basic test for each command is provide in the TestAutomatic.hpp file. To perform a test pass the name associate with the 
test in main as a argument when compiling. 

```
$ g++ main.cpp -o main 
$ ./Insert
```

To run the replica as a database do not pass any arguements. 

<!--
### Break down into end to end tests
Explain what these tests test and why

```
Give an example
```

### And coding style tests
Explain what these tests test and why

```
Give an example
```

## 🎈 Usage <a name="usage"></a>
Add notes about how to use the system.

## 🚀 Deployment <a name = "deployment"></a>
Add additional notes about how to deploy this on a live system.

## ⛏️ Built Using <a name = "built_using"></a>
- [MongoDB](https://www.mongodb.com/) - Database
- [Express](https://expressjs.com/) - Server Framework
- [VueJs](https://vuejs.org/) - Web Framework
- [NodeJs](https://nodejs.org/en/) - Server Environment
-->

## ✍️ Authors <a name = "authors"></a>
- [@smithtanium](https://github.com/smithtanium) <!-- - Idea & Initial work-->
- [@jychen36](https://github.com/jychen36) - Partner during ECE 141B
<!--See also the list of [contributors](https://github.com/kylelobo/The-Documentation-Compendium/contributors) who participated in this project.-->

## 🎉 Acknowledgements <a name = "acknowledgement"></a>
-Rick Gessner (Professor of ECE 141A&B)