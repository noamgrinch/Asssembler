/*
Author: Noam Greenshtain
Date: 25/08/2017

Hello,

I have written this projet all by myself. I will explain in a few sentences how it works in general and what it supports.The code wasn't supposed to end up being that long , I didn't plan my work right. Instead of reading all the instructions I started making the assembler. after I finished the first version, I started debugging. I found some testers and then looked into the instructions in the maman and found out that I missed a lot of details or were just wrong, so I had to almost rewrite it the whole thing.That is why there are a few methods which are almost the same but helping for a specific diffirent cases.

In contradiction to my previous mamans, I tried this time to make the amount of input unblocked. That means I do not reads the line into a char array with fgets() , which gives the assembler more flexiblity with amount of characters in a line. That appeared to be uselss after I read that the maximum is 80 charaters for line, but I am happy with the challenge I gave myself.Also, sometimes it uses less memory than using fgets().

In the Project.zip file there are also .as input files and outputs for a check. In addition there are .as testers for errors almost of every kind.

The Object file that is generated is not in a standard binary form. it is in "weird-binary" which we were instructed to do by the course.
it is in quadnary but in letters(a,b,c,d).

The algorithm is as I was instructed to do(first wave and second wave), but I wanted to make it more clear and easier for you to check it so I will explain a little bit more about how the program works:

A.The program is built by six files:
1."AssData.c": contains functions that deals with data related instructions.
2."AssSym.c": contains functions that deals with symbols related instructions.
3."misc.c": contains miscellaneous functions.
4."first.c": contains functions that are related to the first wave flow.
5."second.c": the same as first.c but for the second wave.It means coding into the structions.
6."assembler.c": the main method which managing the whole flow.

B.The program itself works on an if/else waterfall most of the time.Not just the the main flow , but most of the coding and calculating methods.
  ActionOneMethod() and ActionTwoMethod() are calculating the instructions that comes WITH A LABEL. ActionOneOrder() and ActionTwoOrder() is the same but WITHOUT A LABEL. 

C.The main function which reads words into the program from the file is GetWord(). In addition there are GetData() for data arrays and matrixs and    GetString() for strings.GetWord() is a "Stupid" function, it can't detect errors in the text. it's just returns the needed word(skips spaces and tabs).
 GetData() and GetString() are "smart" functions.They can detect if there is an error in the text because they have specific context.
 
D.The program is based on functions that returns integers(booleans)in the coding and calculating flows.That means, That in the headers of the files you can look for the #define's.In misc.c there is a function called CheckErrors(),this function gets a numeral value and prints out the correct errors on the output.
In general every main calculating(how many lines to add to IC or DC and coding flows) flow returns an integer. In the first wave the main calculating method returns an integer to Lines which is how may lines to be added to the IC. If the value is negative then it is a code for an error to print.
In the second wave the coding methods returns 1 if a coding flow finished successfuly, if negative it's an error code to print.

E.Errors1 and Errors2 integers in the main() function are counters for errors. Error1 for the first wave, Errors2 for the second Wave.
 Most of the errors will be found in first wave.If Errors1 is not equal to 0 before getting to the second wave the program does not continue to the second wave.That means, that if there are logical errors like "label not found" it will not be displayed if there are context errors in the first wave.
 
F.I will try to point out things that was up to our decision(if it wasn't then it's not really hard to change it or matters to the whole flow)
	1.The program supports space between matrixs and '['s.That means: let M5 be a matrix so "M5  [r1][r2]" is a valid statement.
	2.The program supports spaces and tabs before any charcter appears in the line. that means, even with ';' and labels pre-space is not an error.
	3.Labels before .entry statements will display a warning.
	4.The program can get through the command line more than one file.
	
G.Main intergers:
    1.Lines(main-first wave) - contains how many lines are needed to add for the IC.If negative then it's an error code to print.
	2.Lines(main-second wave)- If value is 1 then coding flow finished successfuly.If negative then it's an error code to print.
	3.Errors1 + Errors2(main) - Errors counter for the first and second wave.
	4.status(AssData) - returns 1 if data is coded successfuly.If negative then it's an error code to print.
	
	