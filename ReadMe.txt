Aranan Thayaparan Assigment 4 (kinda also 3) 101256040

Assignment 3:
Tiled Backround: I created more backround game objects to make a sufficently large backround, but I wasn't able to implement continuous scrolling before the deadline

Implementing Camera Movement: I used a translation matrix with the player position to modify the view_matrix so that it would stay centered on the player

Implementing Velocity based motion: I created a velocity vector for the player object and gave it a method that would increase the velocity based on the key that was pressed and accoutn for player direction. for the strafe motion I had to get the orthogonal vector from the up velocity vector and the current direction. I limited the velocity to 0.2f at most because I thought 2.0f (what I assume is 2 units per second) was too fast and hard to control.

Assignment 4:
bullets: player is able to fire a bullet with a timer that runs every half a second to limit the fire rate. The bullets are represetnted with the bullet sprite that is included

Bullet lifes: each bullet has a life span of about 5 seconds I think which could be too long since they are long gone from the play area by about second 2 or 3 but I just kept it at 5 because it was easier during my testing to have the bullet destructor messages come in after any potential collision messages.

Bullet Collisions: Bullets use ray circle collisions to determine if the player will hit the bullet and within the same function it calculates t which is like the time till the bullet hits the object and if t is 0 that means the bullet has hit the object and then it deletes it. I found a bug  when the bullet sprite was larger that if the enenmy was patrolling (has circular motion) and the bullet doesn't hit the enemy straight on, but rather the enemy will run into the bullet from the side then nothing will happen and it will be treated as a miss. This is honestly not a bad thing because the player techincally missed and when I made the bullets a little smaller then its not really noticable because it now looks more like a near miss rather than a missed collision [so I'm going to take this as a win and say that its an intended feature :)]

Distribute enemies: So I basically just took the enemies I had already created and made their spawning position random to a certian degree so that they will be random but they should still be on the backround. I didn't fully implement assignment three so I don't have the scrolling backround so I just tried to keep them on the repeated backrounds I created just to extend the play area

Particle System issues. I wasn't able to figure out how to implement the particle systems propperly: when I added the code from the demo to try and get that working I ran into an issue somewhere in the libraries I had downloaded. something about a static cast not being valid from a type B to a type T I had never run into this issue before and when I removed the particle demo stuff it was working fine. I will try and see if I can get help at my next tutorial session, but for now I am stuck.

Heirarchical Transformation: I was able to get the blade spinning and moving with the player but I think I did it kinda wierd, I am fairly certain it is a heirarchical transformation though you might have to read through the extra render I made that involves an additional prameter


Additional Note regarding Assignment 3: I emailed the prof before the due date for assignment three but never recieved a reply, so I'm just going to explain briefly here and if there is anything that can be done I would greatly appreciate it. I am currently searching for co-op positions this summer and I had received a sudden influx of 3-4 interviews that were scheduled for reading week and the week after and I wasn't able to spend as much time on assignment three as I would have liked. As you may or may not have noticed the first few sections are things from assignment three that I had implemented before the due date, but I never ended up submitting the assignment. I understand that I missed the due date and there is probably nothing that can be done but if the TA that marks this can do anything to add those top few marks to assignment three or maybe add them to this asssignment I would greatly appreciate it. 


