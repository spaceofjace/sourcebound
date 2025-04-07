# Arkanoid/Breakout Research
*Last Update:* Apr 2, 2025
## Purpose

In being tasked with building an Arkanoid-like clone, I obviously need to spend more than a little time getting familiar with games of that style.  I don't expect to be able to design a "better" Arkanoid, but it will help me develop some initial thoughts or considerations for development, as well as think through how gameplay might be expanded by someone with more gameplay design experience. (Not that I _can't_ design a game, or don't have game design skills, but I don't claim to be an expert by any stretch.)

## Goals and Objectives

Not comprehensive, of course, but at the highest level, I want to: 

* Understand core gameplay concepts
* Identify core gameplay mechanics (control, physics)
* Think through how levels should be designed
* Understand deeper gameplay mechanics (power-ups, etc)
* Investigate variations among Arkanoid clones to better understand how systems might evolve

## Methodology

**Read about original Arkanoid** -> This gives me some insights into the history and development of the game, as well as some of the general gameplay mechanics.

**Play some free online version(s), if available** -> My goal is to spend roughly 30 minute to 1 hour playing the game every day I am working on it; there's few things I can think of that can give you a more solid understanding than spending time with the game itself.

## Findings

### Core Mechanics Overview

* Essentially a one-player "pong" game
* Paddle moves left and right
* Ball bounces off paddle, bricks, and sides of map
* Different types of bricks presented that are (usually) destroyed by the ball upon contact
* Player has lives, and lives are lost if ball hits bottom of screen (below paddle)
* In Arkanoid (vs. Breakout), ball speed increases over number of hits or time, increasing difficult for player

### Known Mechanics from Original

I found one version of the original game available online, but it is unclear if the game is officially licensed by the publisher.  (It also requires watching an ad to play; I did play this a few times, though, just to get familiar.)  These are more detailed mechanics from what I was able to discover:

#### **Brick Types**

Each brick has a different rule set, and these are primarily how levels can be controlled for difficulty

Types:
* Standard - breaks in one hit
* Unbreakable - can't be destroyed (player obstacle)
* Multi-hit brick - requires multiple hits (2?) to destroy
* Regenerating bricks - I didn't see this in gameplay, but sources indicate later levels sometimes had regenerating bricks

#### **Power-Ups**

Most power-ups overwrite the previous one, meaning only one could be active at a time.  Worth noting there are instant power-ups that this doesn't apply to, such as extra lives.  Similarly, a different game design could allow for some power-ups to be simultaneous, such as one paddle power-up and one ball power-up.  

Power-up drops appear to generate randomly.

Types:
* S - Slow, slows down the ball
* L - Laser, allows the player to shoot lasers from teh paddle to destroy bricks
* E - Expand, increases paddle size
* C - Catch, can capture ball with paddle and release manually
* D - Disruption (multi-ball), splits ball into 3
* B - Break, instantly advance to next level
* P - Player, an extra life
* V - Vaus, skips you to the final level (rare)

#### **Other Notable Mechanics**

* Paddle moves left and right
* Player starts with three lives total.
* Ball bounces off paddle, bricks, and sides of map; bounce angle is at least partly based on where the ball hits the paddle rather than true bounce angles
* Different types of bricks presented that are (usually) destroyed by the ball upon contact
* Player has lives, and lives are lost if ball hits bottom of screen (below paddle)
* In Arkanoid (vs. Breakout), ball speed increases over number of hits or over time in later levels, increasing difficult for player

### Notes From Variant Games

#### Other Power-Ups

From https://play.arkanoid.online/ 

* Fast ball
* Double ball (technically different from D power-up)
* Tiny ball
* Fire ball, explodes multiple bricks
* Zap bricks 
* Hard ball, breaks bricks it passes through
* Shrink paddle
* Sudden death, lose a life
* Explode all
* Multiply exploding

### Initial Thoughts/Questions

* Some game data should be tunable, such as ball speed, base acceleration rate, paddle speed
* Obviously, it would be good to have a plan of implementation for all the base power-ups, but realistically, I may not have time (with my already lofty ambitions)
* I will need to spend more time with gameplay to see if there are exceptions to only one power-up active at a time (even in the base game)
* I will research more about scoring, since this is clearly a core gameplay component

## Reading Sources/Materials

* Arkanoid Wikipedia, https://en.wikipedia.org/wiki/Arkanoid -> useful top-level summary
* Arkanoid StrategyWiki, https://strategywiki.org/wiki/Arkanoid -> alternative (detailed) source of mechanics
* Arkanoid Arcade Game – Breaking Barriers in Gaming, https://bitvint.com/pages/arkanoid -> history and gameplay

## Online Gameplay

* https://www.classicgame.com/game/Arkanoid - unclear if this is reproduced with permissions, but it appears to be a copy of the original game
* https://play.arkanoid.online/ - looks like a Russian developed clone based on peeking at the page metadata (Andrey Isaev?)