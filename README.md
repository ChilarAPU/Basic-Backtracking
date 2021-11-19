# Basic-Backtracking
Covers a fundamental recursive backtracking algorithm in UE4 C++.

Bugs Identified in Current Version:
End Paths Set does not work correctly. It will sometimes choose the right tile. Debugging shows that the NeighbourCount function gives out the wrong value which causes this bug

What Needs to be Completed: 
Floor and Walls currently spawn but it can be made better. Use CheckNeighbourLocation() function on each path tile and spawn a wall if it returns false. This will stop walls being spawned where they are not needed (where the player wont be able to see them)
Add some more customisability to the maze e.g. dictate how many times a maze can turn.
