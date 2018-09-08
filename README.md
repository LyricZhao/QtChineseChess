### Happy Qt Chinese Chess

#### Introdution

- A Qt-based online Chinese chess game platform.
- Developed by LyricZ, 2017011362.
- A summer programming training project, 2018 Summer.
- Github: https://github.com/LyricZhao/QtChineseChess

![](http://otxp6khet.bkt.clouddn.com/qcc/updc2.jpeg)



#### Features

- [x] OOP Design Style, Only 1.2k LoC.
- [x] High Scalability
- [x] High-Efficient Implement
- [x] Cross-Platform
- [x] Colorful Display
- [x] Sound Effect
- [x] Smooth User Experience
- [x] File Save/Load
- [x] Comfortable Coding Style
- [x] Tcp High Stability



#### Environments

- Local Compiling Environment

  - macOS 10.13.4
  - Qt 5.10.1
  - Clang 902.0.39.1

- I don't compile the code on Windows or Linux, I am not sure whether there will be some problems.


#### How to game

- About the game, there are 3 global status of the system:

  - Not connected, you can do nothing but to connect.

  - Pending, load file or get ready for the game.

  - Gaming, enjoy and focus on your game!

- ToolBar

  ![](http://otxp6khet.bkt.clouddn.com/qcc/updc6.png)

  - Connect

    - You can choose to be the client or the server.

    - When you choose to be client, you should input the IP address and the port of the server, if server, you just should input the port you want to listen.

    - In fact, the client and server are equal but only that the server can listen and wait for the client to connect.

      ![](http://otxp6khet.bkt.clouddn.com/qcc/updc1.png)

  - Disconnect

    - When you are in the status of Pending or Gaming, you can click it and disconnect the current connection.

    - When you click, you will quit the current status to Not connected status, but the status of chessboard will be saved and you can save it into a file.

  - Ready

    - This function can only be used in Pending mode, click it and it means that you are satified with the chessboard and ready for the game, the status of the players will be displayed.

    - Click again to cancel the ready status.

    - When two players are all ready for the game, the game will start.

    - The player which will move first is the one who gets ready first.

  - Give Up

    - This function is only available in Gaming mode.

    - Clicking this means that you surrender and quit the Gaming status and back to the Pending status.

  - Load file

    - Load a chessboard file from the disk when you are not gaming.

    - If you are in the Pending mode and load a file, your board will be exactly set to the file and another player will be the another side of the status as the file describes.

    - It is very fair because the game can only be launched when the two both accept and are ready for the game.

  - Save file

    - You can do it anytime!

    - The file will be saved into the path you choose.

    - Note that the timer (when gaming) will not be stopped when you are saving a file.

  - Reset

    - You can reset the chessboard back to the default chessboard by just clicking it in Not connected or Pending mode.

    - In fact, to simplify the code, the Reset button is just an implement of loading file of  the default status.

  - Settings

    - You can change the settings anytime!

    - The username(in fact no use..) and the sound effect can be set here.

    - Note that the timer (when gaming) will not be stopped when you are setting.

      ![](http://otxp6khet.bkt.clouddn.com/qcc/updc3.png)

  - Help me

    - Help the poor programmer!

      ![](http://otxp6khet.bkt.clouddn.com/qcc/updc0.png)

  - Quit 

    - Quit the game anytime!

- Note that the timer is limited to 20s, which means that you need to move in 20s for a single step.



#### About the Code

- I think the code is pretty OOP-Style and comfortable to read, all functions are implemented in only 1.2k LoC.

- The complex rules in Chinese chess are implemented in only about 1h LoC using a lots of macros and elegant expressions, e.g. (The implement of getting the reaching points of a chariot)

  ```c++
  # define funcChariot if(!inRange(rx, ry) || isSame(x, y, rx, ry)) break; \
      ret.push_back(MP(rx, ry)); \
      if(exist(rx, ry)) break; \
  
  std:: vector<IPoint> Rule:: reachingChariot(int x, int y, int p) {
      (void)(p);
      std:: vector<IPoint> ret; ret.clear();
      for(int rx = x - 1, ry = y; ; -- rx) { funcChariot; }
      for(int rx = x + 1, ry = y; ; ++ rx) { funcChariot; }
      for(int rx = x, ry = y - 1; ; -- ry) { funcChariot; }
      for(int rx = x, ry = y + 1; ; ++ ry) { funcChariot; }
      return ret;
  }
  ```

- Also, there are some lambda expression in C++ 11 to simplify the code.

  ```c++
  connect(client, &QTcpSocket:: disconnected, this, [this](){ emit connectLost(); });
  ```

- Almost all the functions are controlled in 10 lines and all the key components are packaged in a class ensuring the scalability.

- The relationships of the components are as below:

  ![](http://otxp6khet.bkt.clouddn.com/qcc/updc5.png)	

- Here are the details of the design:

  - communicator.cpp/h

    - This file is about the network connection and the send/receive data functions are also implemented in the file.
    - The implement uses QTcpSocket and QTcpServer, the behaviors of the server and client will be exactly same after they establish a connection and it's esay for debugging.

    - Here I also consider the situtation that message is splitted or sticky to one, and also implement the error handler when losting connection.

    - The file also implement the connection dialog and can check the correctness of the IP address.

  - helpme.cpp/h

    - The implement of the "Help me" dialog.

    - Pretty interesting.

  - rule.cpp/h

    - A filter which can return the reaching points of a selected chess piece.

    - You can create one with the chessboard status and input the position, the class Rule will return a std:: vector showing where the piece can go.

  - setting.cpp/h

    - The setting dialog implement.

  - statusengine.cpp/h

    - Almost the core of the game.

    - The class StatusEngine save the status of the game and is responsible for drawing the chessboard.

    - Also, the file saving/loading functions are also implemented in the class.

  - mainwindows.cpp/h

    - We can say that the class MainWindow is a bridge for all the components and connect them into a completed game.

    - The code is not very complex because the main work is finished in the sub-working classes.

    - The most important work of the main window is handling the events such as the paintEvent or MouseMove, etc..

    - What I want to highlight is that the movement of the chess piece can be finished by dragging the chess and the target legal places to move will be highlighted which is not very easy to code.

      ![](http://otxp6khet.bkt.clouddn.com/qcc/updc4.png)



#### About the Interface and Sound Effect

- The icons of the pieces and chessboard are downloaded from the Internet.
- The background is also downloaded from the Internet, but the shadow effect of the board is made by myself using PhotoShop,  I spent a lot of time on the details.
- The sound effect is extracted from QQ Dou Di Zhu, that why I call the game "Happy" Chess, I think building a game like it is pretty interesting (although a little bothering about the complex code structure)



#### Thanks

- Thanks to my roommate and TA to help me test the game!



#### Enjoy the game!