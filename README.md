While creating videogames we will notice that they are full of expensive operations that needs a high amount of resources. Draw calls (objects, characters, maps), collisions... These actions can bring down our game performance as the number of game objects increases.
Think in a game where it's necessary to check the collisions between objects every frame. If there are 5 objects on the scene, the game needs to check every pair of them. 5 objects is not a high number of checks (5 objects * 4 other objects = 20 checks), but what will happen if the scene have 50 objects? 50x49 = 2450 checks, and 100 objects? and 200?... That's a lot of checks every frame.

![](http://i65.tinypic.com/51oug9.png)

To prevent these problems we need to reduce these operations. Are collision checks necessary if the objects to check are in diffrent parts of the screen? Or necessary to check if every object is inside the screen so we can draw it? Wouldn't be better if we only check the collisions if the objects are near? Or check the ojects inside the screen and draw them skipping the ones outside?

One way to reduce the operations needed in those situations is using **Space partitioning**.

## [](#header-2)Space Partitioning

Space partitioning systems divides the space into regions and then the same system is recursively applied into these new formed regions. These regions can be organized into a **Tree**.

The common **Trees** used in videogames are the *BSP tree, Octree and k-d tree* for 3D games and *Quadtree* for 2D games.

## [](#header-2)Quadtree

Quadtree is a tree data structure used to divide a 2D space into 4 nodes(childs) and recursively subdivide each child node into 4 more. It's an extended version of **Binary tree** or a two-dimensional analog of **Octree**.


![](http://doi.ieeecomputersociety.org/cms/Computer.org/dl/trans/tp/2009/07/figures/ttp20090711781.gif)
###### [](#header-6)You can see how the space is divided in 4 nodes and how it looks in a tree form.

The quadtree starts as a single node. The objects added on the quadtree are added to this single node and when the maximum objects per node (specified by you) is reached, it will split into 4 childs creating a new level in the tree and place each object to the corresponding child depending on their place in the 2D space.

![](https://upload.wikimedia.org/wikipedia/commons/thumb/8/8b/Point_quadtree.svg/450px-Point_quadtree.svg.png)
###### [](#header-6)The white cercles represents the objects.

The objects that fits entirely inside the new nodes are pushed down to the next level (child node), instead, if doesn't fit, they are remain in the same level (parent).

![](http://www.kyleschouviller.com/wp-content/images/_quadTree00.jpg) 
###### [](#header-6)The object is inside level 2 of the three, while level 1, the parent, is the entire box.

![](http://www.kyleschouviller.com/wp-content/images/_quadTree01.jpg)
###### [](#header-6)If doesn't fit, the object remains at parent.

As you can see this method groups the objects that are near in the same node and the ones that are far from them on others. That means that is not necessary to perform expensive operations like collision checks between distant objects, only in the objects that are placed in the same node.

Here you can see an example:

![](http://i65.tinypic.com/2wps1tu.gif)

## [](#header-2)Exercise

### [](#header-3)TODO 1

### [](#header-3)TODO 2


