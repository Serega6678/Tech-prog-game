# TP-homework

Attacking and defending fraction fight between each other.

The game is designed for 2 players.

Firstly, the first player chooses whether he wants to attack or defend, then the second player gets the remaining side. Afterwards players create an army.

The game is played on the playing board of size = 8.

At the beginning of each turn, the player either changes the position of his soldier, attacks the enemy soldier or regroups his army.

The goal of the attacking team is to kill the leader of the defending team, the goal of the defending team is to kill all the attackers.

Patterns used:

        1 Creational:

            * Singleton           to have only one playing board
            
            * Abstract Factory    to make a smart army creation
            
        2 Descriptional:
        
            * Decorator           to notify the player when the soldier is created
            
            * Composite           to be able to group several soldiers together in the squad
            
        3 Behavioral:
        
            * COR                 to make smart and adjustable gameflow
            
            * Visitor             to diplay playing board

Also you can find the architecture of my game in Scheme.png file. 

In Finished Game folder you can find the finished version of the game. In Game to test folder you can find the same version of the game and the testing script to prevent any bugs appearing in the final version.

installing script.sh installs everything you need to play the game, building script.sh builds the game and checks if all the tests were successfully completed.

Alternatively, you can use play game.sh script to build the final game version to avoid running any unit tests.
