While creating videogames we will notice that they are full of expensive operations that needs a high amount of resources. Draw calls (objects, characters, maps), collisions, locate objects...

These actions can bring down our game performance as the number of game objects increases.
Think in a game where it's necessary to check the collisions between objects every frame. If there are 5 objects on the scene, the game needs to check every pair of them. 5 objects is not a high number of checks (5 objects * 4 other objects = 20 checks), but what will happen if the scene have 50 objects? 50x49 = 2450 checks, and 100 objects? and 200?... That's a lot of checks every frame.

![](http://i65.tinypic.com/51oug9.png)

To prevent these problems we need to reduce these operations. 

Are collision checks necessary if the objects are in diffrent parts of the screen/world? 
If we are creating a game with armies of hundreds of units fighting with other armies and need to attack the near enemies, is necessary to iterate through all the scene units? Wouldn't be better if we only take into account if the objects are near?

For sure this will reduce the operations we need to do every frame.

One way to reduce these operations needed in those situations is using **Space partitioning**.

## Space Partitioning

Space partitioning systems divides the space into regions and then the same system is recursively applied into these new formed regions. These regions can be organized into a **Tree**.

The common **Trees** used in videogames are the *BSP tree, Octree and k-d tree* for 3D games and *Quadtree* for 2D games.

In this research i want to focus on 2D games so i'll talk about **Quadtrees**. 

## Quadtree

Quadtree is a tree data structure used to divide a 2D space into 4 nodes(childs) and recursively subdivide each child node into 4 more. It's an extended version of **Binary tree** or a two-dimensional analog of **Octree**.


![](http://doi.ieeecomputersociety.org/cms/Computer.org/dl/trans/tp/2009/07/figures/ttp20090711781.gif)
##### You can see how the space is divided in 4 nodes and how it looks in a tree form.

The quadtree starts as a single node. The objects added on the quadtree are added to this single node and when the maximum objects per node (specified by you) is reached, it will split into 4 childs creating a new level in the tree and place each object to the corresponding child depending on their place in the 2D space.

![](https://upload.wikimedia.org/wikipedia/commons/thumb/8/8b/Point_quadtree.svg/450px-Point_quadtree.svg.png)
##### The white cercles represents the objects.

The objects that fits entirely inside the new nodes are pushed down to the next level (child node), instead, they remain in the same level (parent).

![](http://www.kyleschouviller.com/wp-content/images/_quadTree00.jpg) 
##### The object is inside level 2 of the three, while level 1, the parent, is the entire box.

![](http://www.kyleschouviller.com/wp-content/images/_quadTree01.jpg)
##### If doesn't fit, the object remains at parent.

As you can see this method groups the objects that are near in the same node and the ones that are far from them on others. That means that is not necessary to perform expensive operations like collision checks between distant objects, only in the objects that are placed in the same node.

Here you can see quadtree in action:

![](http://i65.tinypic.com/2wps1tu.gif)

## Documentation

The theory and the images above can be found in the following links:

* [Wikipedia](https://en.wikipedia.org/wiki/Quad_tree)

* [Quadtree implementation in javascript with example](http://www.mikechambers.com/blog/2011/03/21/javascript-quadtree-implementation/)

* [2D collisions with Quadtrees](https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374)

* [More info of Quadtree used in 2D collisions](http://www.kyleschouviller.com/wsuxna/quadtree-source-included/)

* [Information about Quadtree](http://blog.notdot.net/2009/11/Damn-Cool-Algorithms-Spatial-indexing-with-Quadtrees-and-Hilbert-Curves)

# Implementation (c++)
Now that you know that **Quadtrees** can be helpfull to reduce operations needed we will see how to implement them in our project following an **Exercise** 

## Exercise
In the link below you can download a Visual Studio 2015 project with a simple 2D Engine made using SDL2 and the necessaries TODO's.
You can also find a solution to see how it's the result after all the TODO's are finished.

[Download the Exercise](https://github.com/TinoTano/Personal_Research/releases/download/1.0/Exercise.zip)

Below you can see each TODO steps.
For this exercise we will use the quadtree to optimize the collisions between characters. 
In the following lines i'll refer to the character colliders as objects because quadtree can be used for other things (not just colliders) and the implementation is the same.

_Inside Containers folder you can find the files QuadTree.cpp and QuadTree.h._

### TODO 1

#### Declare the class QuadTree

(#define) the max objects per node before split and the node count (this is obligatory 4, remeber that we are implementing a quadtree).

Declare the constructor and the following functions:
_The following functions names are just examples._

* **Constructor(...):** Receiving the first node Rect and initializing all the variables.
* **ClearTree():** Clear the tree by clearing recursively all nodes.
* **SplitNode():** Split the node into 4 childs and assign the corresponding index to each child.
* **GetNodeIndex(...):**: Receiving the object and returning the node index where the object is.
* **InsertObject(...):** Receiving the object to insert into the corresponding node.
* **Retrieve(...):** Receiving a reference of list/vector to fill with the collisions and the collider to check the collisions.
* **GetNodes(...):** Receiving a reference of a list/vector to fill recursively with all the nodes inside the tree. This will be used to draw the full quadtree.
* **(Optional) Contains(...):** Receiving a object to check if it fits entirely inside the node. This is optional because you can check this inside the **GetNodeIndex(...)** or call this function from **GetNodeIndex(...)**. This is up to you.

Declare the variables:

* **list/vector** to store the objects
* The node **Rect**.
* An **Array** of 4 quadtree (each element will be a child node)
* An **Integer** to store the index of the node.

### TODO 2

#### Define the class QuadTree

Define the constructor:

* **Constructor(...):** Assign the received Rect to the node rect variable and initialize all the variables.

Define the following functions:

* **ClearTree():** Clear the tree by clearing(nullptr) recursively all nodes and clear the object list/vector.
* **SplitNode():** Calculate the Rects of the new nodes, create the new nodes with those rects, assign the new nodes to the corresponding elements in the array and give the index number to each node inside the array. 
* **GetNodeIndex(...):** Check if the object fits entirely inside any of the 4 nodes and return the node index or -1 if doesn't fit.
* **InsertObject(...):** Check if the node have any child and insert the object there. If there are no childs or doesn't fit, add the object to the parent. When the object is added, check if it's necessary to split the node. If the split occurs, the node will insert the objects to the childs if they fit, if not they remain in the parent. 
* **Retrieve(...):** Fills the given list/vector with all the potential collisions in all the nodes with the received object. **!!Important!!** Make sure to skip it's own collision or we will check 1 extra useless collision.
* **GetNodes(...):** Fills the list/vector with all the nodes in the tree to draw them. This is just for visual purposes.

Now the Quadtree class is done but if we want to use it we need to create it. In our case this will be created inside the Collision module.

### TODO 3

_All the steps will be done inside Collision.cpp and Collision.h. You can find them inside Modules folder._

* Declare a variable of type Quadtree (the class we created before) and initialize it with the screen Rect.
* Declare a list/vector to store the collisions.
* Declare a list/vector to store the tree nodes to draw them later.

All the next steps are done in the Update function.

* First of all Clear the quadtree because the nodes can change in each Update.
* Now that we have a clean tree, insert in the quadtree all the existing objects in the scene.

After the quadtree have all the colliders, we have to iterate through each collider.

* While iterating we need to clean the collisions list/vector because as before with nodes, the potential collisions can be different.
* Now it's time to retrieve the potential collisions for the current collider and store them inside the collisions list.
* If we found potential collisions then we need to iterate throught them and check if they are colliding with the initial object. **Note:** You can do it like in brute force method. **Note2:** Remember to increase the value of quadTreeChecks to see the result on the tittle screen.

We are finish with the QuadTree implementation for collisions!

See how the collision checks and the time needed for each method is reduced:

![](http://i64.tinypic.com/23lgy1l.png)

As you can see when we have low objects to check, the time needed is similar. Why? Because in Quadtree method we need to clear the tree and retrieve the potential colliders while the brute force method goes straight. But look the time when the number of objects is high. Here you can see a big difference. 

So, are these methods always better? If you need to perfom a high number of operations, absolutely yes.

### TODO 4

Let's make it more visual!

While you are iterating throught the potential collisions, after/before checking if potential collisions are colliding with the initial object, add these calls:

* App->render->DrawQuad() with the potential object collision Rect, the desired color for the Quad and set _filled_ to false.
* App->render->DrawLine() with the initial object position, the potential object collision position and the desired color for the line.

We can see which objects have potential collisions and a line to know with which one.

But we also want to see the tree nodes!

_We can place the following code at the end of the Update function._

Here is where we will use the GetNodes(...) function and the node list/vector we have created before.

* Clear node list/vector.
* Call the function GetNodes() with the node list/vector to fill.
* Iterate throught the list nodes and call App->render->DrawQuad() with the node Rect, desired color and set _filled_ to false.

![](http://i63.tinypic.com/35jkvv6.png)

Now it's done. We can see the the potential collisions and the Quadtree nodes!

### TODO 5

We have the Quadtree working and we can visualize it, so let's see it in action!

* Uncomment the lines for this TODO inside the Update function in EntityManager.cpp.

This is the result:
![](https://i.gyazo.com/a88e900ca459a8269559542d945df55d.gif)
##### Nodes dividing when more than 2 objects are inside.

![](https://i.gyazo.com/f4e49ac4a1762b4e536b4a61e48d8f86.gif)
##### Nodes modifying when objects enter/leave nodes.

### Extra

You will see that some objects that are near are not set as potential collisions. That's because they are in a different node. Remember that if the objects doesn't fit **Entirely** in node Rect it will remain in the parent.

* Change the code so the objects that doesn't fit in the child nodes will be placed on each child they itersects instead of placing it in the parent.

If you did the last change, you will see that now nodes are dividing more times than before (they are creating more tree levels). It's good to place a max number of levels to skip node division if it is reached.

* Implement a max level for tree to prevent excessive node division.

## Contact

Feel free to contact me at _elsuperchowchow@gmail.com_
