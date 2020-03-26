# Simple-Chess-UE4-Cpp
Classical strategy board game - Chess.

#Description
Whole chess set are derived from “base chess” class which contains general objects, variables and methods,(e.i. movement, material blending, events). Then derived classes override some specific for them behavior, in this case: movement patter for all, exclusively for  pawn - possibility to become a Queen and King - call the end game event on destroy.

Since it's a game for 2 players, the proper way is to create 2 character players instead of 1 (as i made). The reason why i did it - i didn't set a point to crate a game with AI opponent and avaible to play over internet. Also, it's save some time during code writing.
