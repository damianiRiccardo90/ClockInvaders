# ClockInvaders
A small 2D project about shooting floating (and bouncing!! OMG) destructible clocks.

# Specs:
1.  The cannon (a triangle) has to be placed to the middle of the bottom part of the screen (window).
2.  The cannon should smoothly turn with the help of the cursor keys (to the left, to the right) in a range of -90 to 90 degrees from the basic direction (the basic direction is upwards).
3.  Pushing the SPACE key makes the cannon shoot a projectile: A small object consisting of lines. The projectile flies in the direction the cannon is turned to.
4.  It has to be possible to shoot several projectiles if the player holds the SPACE.
5.  Create an object (a clock with hands), approximately with dimensions of 100x100 pixels, which shows the real time.
6.  Display two clocks at the same time at random spots on the screen; both have to show time all the time.
7.  Set the clocks in motion at random speed and direction. When bumping into a screen margin, the clocks rebound; when bumping into each other both the objects rebound away from each other (the direction of their movement in x and y reversed as appropriate).
8.  Make sure there is a testing of collisions of the clocks with the projectiles shot from the cannon. When the clocks are shot down, they change into two smaller clocks and the projectile disappears. If the too small clocks are shot down, they disappear completely (small = approximately of 5x5 pixels).
9.  Once the last clocks are shot down, the two big clocks appear again (see point 6) and the whole procedure repeats.
