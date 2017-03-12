While creating videogames we will notice that they are full of operations that spwnds a high amount of resources. Draw calls (objects, characters, maps), collisions... These actions can bring down our game performance as the number of game objects increases.
Think in a game where it's necessary to check the collisions between objects every frame. If there are 5 objects on the scene, the game needs to check every pair of them. 5 objects is not a high number of checks (5 objects * 4 other objects = 20 checks), but what will happen if the scene have 50 objects? 50x49 = 2450 checks, and 100 objects? and 200?... That's a lot of checks every frame.

To prevent these problems we need to reduce these operations. Are collision checks necessary if the objects to check are in diffrent parts of the screen? Or necessary to check if every object is inside the screen so we can draw it? Wouldn't be better if we only check the collisions if the objects are near? Or check the ojects inside the screen and draw them skipping the ones outside?

One way to reduce the operations needed in those situations is using **Space partitioning systems**.

#Space Partitioning

Space partitioning systems divides the space into regions and then the same system is recursively applied into these new formed regions. These regions can be organized into a **Tree**.

The common **Trees** used in videogames are the *BSP tree, Octree and k-d tree* for 3D games and *Quadtree* for 2D games.

