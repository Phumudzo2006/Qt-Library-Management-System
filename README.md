📚 Qt Library Management System

A desktop-based Library Management System built in C++ using the Qt Framework.
The application provides a graphical user interface for managing books, users, and borrowing records, simulating how real-world library systems operate.

This project was built to strengthen my GUI development, object-oriented programming, and event-driven software design skills in preparation for software engineering internship roles.

🧠 Project Overview

The Qt Library Management System demonstrates how desktop applications can manage structured data through an intuitive interface.
It models real library workflows such as book registration, borrowing, returning, and record tracking.

The project focuses on:

Clean GUI design

Separation of UI and business logic

Reliable state updates using Qt’s event system

🛠 Technologies Used
Technology	Purpose
C++	Core application logic
Qt Framework	Application structure and lifecycle
Qt Widgets	GUI components (buttons, tables, forms)
Signals & Slots	Event-driven communication
Data Structures / File Handling	Record storage and updates
✨ Key Features
📖 Book Management

Add new books

Remove existing books

View available books

Search books by title

👩‍🎓 User Management

Register students/users

View stored user records

🔄 Borrowing System

Issue books to users

Return borrowed books

Track borrowing status

🖥 Graphical User Interface

Interactive buttons and forms

Dynamic table updates

Dialog messages for feedback

🏗 System Design

The system is structured using:

Classes to represent books and users

STL containers (e.g. vectors) to manage records

Qt Signals & Slots to synchronize GUI actions with backend logic

Modular functions for each operation (add, borrow, return, search)

This design ensures maintainability and scalability.

▶ How to Run the Project
Requirements

Qt Creator

C++ compiler (MinGW or MSVC)

Steps

Clone the repository

Open the .pro file in Qt Creator

Click Build

Click Run

🖼 Example Functionalities

Adding a new book via form input updates the table instantly

Issuing a book changes its availability status

Returning a book restores availability and updates records

🚀 Future Improvements

Database integration (SQLite / MySQL)

User authentication and roles

Late return fine calculation

Barcode scanning support

Dark mode UI

🎓 Concepts Demonstrated

Object-Oriented Programming (OOP)

GUI application architecture

Event-driven programming

Data and state management

👨‍💻 Author

Phumudzo Matshaya
Software Development • C++ • Qt GUI Applications
