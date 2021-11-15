# Basic-Backtracking
Covers a fundamental recursive backtracking algorithm in UE4 C++.

Bugs Identified in Current Version:
End Paths Set does not work correctly. It will sometimes choose the right tile. Debugging shows that the NeighbourCount function gives out the wrong value which causes this bug

What Needs to be Completed: 
Floor and Walls need to be spawned in instead of the temporary plane that currently exists
Add some more customisability to the maze e.g. dictate how many times a maze can turn.
