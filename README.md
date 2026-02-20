Approach and Decisions
======================

Leap and Slam
-------------

Initially my approach involving input and behaviour for leap and slam, thought of creating a placement marker that appears when the ability is initiated which can be controlled with mouse cursor. When player presses leap input then the ability will be triggered, player jumping and landing on the placement marker. But after seeing POE reference and having played Diablo and Torchlight barbarian class, I dropped placement marker approach.

So, when player pressed leap input, player's position will be considered as start position and mouse cursor location will be taken as target location. The process behind taking mouse location involves, projecting mouse position on world and use nav mesh to get the target location fesability from the mouse's hit world location. If the mouse is not hitting ground then the ability will be cancelled, not invoked. The leap has a distance limit, player wont leap exactly to the mouse location. If the target location is beyond this distance limit, then from the start position and mouse hit position a direction vector will be calculated and a new position(threshold position) will be found which is within the distance limit.

Suppose the mouse position is valid but the threshold position is not valid, say a gap is present in between, then short interval will be decremented with the threshold location repeatedly towards the player till a proper position is found, so that the player will leap to safest edge position(new threshold position instead of existing threshold position on the gap). Only when the proper position is found, the cost and cooldown of the ability will be applied. No proper landing position then leap ability will not be committed and will just be cancelled.

THe leap ability class doesn't have a base class, as other than keeping common parameters in ability base class, it was not needed for the scope of this test. Also didn't create any data assets for the ability(hold all necessary details in) since the ability blueprint itself gathers all the necessary data.

In terms of C++ and blueprint combination, I implemented all the ability related logic in C++ itself only used blueprint to expose parameters to the designers. Wrote the arc movement logic in a separate ability task, so that it can be used for cases as well. I used spline component to simulate the arc movement pattern, but as a future update I would like to use Bezier curve equation to simulate arc pattern, as it offers more control over the arc movement enabling more options to the leap game design. I have used it before in one my previous works.
Rest of the things like UI to show health and mana attribute, attribute reset in level, triggering leap ability through input(in player character), spawning UI and binding attribute callback(in player controller) are done in blueprint.

Have used gameplay effect to apply mana cost and cooldown to the player once leap is performed. Its in the leap ability blueprint itself, also used gameplay tags for this.

The leap parameters like leap height, distance, etc are exposed in the ability blueprint allowing modification.

The initial values of the attribute health, mana and attack speed are set in default game effect and gets applied in the player character in PossessedBy(), leap ability is also granted here. The attack speed values can be changed here, the ability also gets this value during the leap calculation.


Player Controller, Player Character and Player State classes
------------------------------------------------------------

So I placed the WASD movement logic and input binding for that in player character(C++), as character has the movement component and different characters may have different movement requirements.

In Player controller, I have placed logic for player attribute change callbacks to update UI(C++ & blueprint). As controller class can posses several characters and the attribute(buff, etc) may need to persist across multiple characters.

The Ability system component is attached to player state instead of other classes, as it persists and so cooldowns reset, buffs vanish, attribute state resets can be done easily, these have to be repeatedly done on death and spawn if attached to character. Also considering multiplayer, player state is the best place to attach ability things.


================================================================================================================

Core Classes

================================================================================================================

GAS_abilitiesCharacter

Player character class responsible for movement and interaction with the Ability System. Default input mappings are added and mapped here. Attributes and ability are applied to ability system in this class.

================================================================================================================

GAS_AbilitySystemComponent

Custom Ability System Component used to extend GAS functionality. It's empty as of now, only holds owner.

================================================================================================================

GAS_AbilitiesPlayerState

Stores Ability System and Attributes on PlayerState. Getter and Setter for ability component and attribute.

================================================================================================================

GAS_abilitiesPlayerController

Attribute change callbacks to updates UI when:

Health changes

Mana changes

I wrote the attribute callbacks in player controller class instead of player character , because the player controller can posses any other character but the attributes needs to retain across many characters.

================================================================================================================

Attribute System

PlayerAttributeSet

Defines player stats used by GAS.

Variables:

Health

Mana

AttackSpeed

================================================================================================================

Gameplay Ability

GA_LeapSlam

Gameplay ability that launches the player into the air and performs a slam attack.

Variables:

Leap Settings

MaxDistance – Maximum leap distance.

BaseDuration – Leap duration.

ArcHeight – Height of the leap arc.

AoERadius – Area damage radius.

Slam Settings

SlamMontage – Animation played on landing.

LandingCameraShake – Camera shake effect.

LandingVFXCueTag – Gameplay cue for landing VFX.

DamageEffectClass – Damage GameplayEffect.

Radius – Slam damage radius.

KnockbackStrength – Horizontal knockback force.

UpwardForce – Vertical launch force.

DebugSlamLanding – Debug visualization.

Functions:

ActivateAbility()
Starts the leap ability and launches the character.

EndAbility()
Ends the ability and cleans up tasks.

CalculateLandingLocation()
Determines where the player will land.

ApplyAoEDamage(Location)
Applies damage and knockback to nearby enemies.

OnArcFinished()
Triggered when the leap movement completes.

================================================================================================================

AT_LeapToLocation

File: Task/AT_LeapToLocation.h / .cpp

Custom Ability Task that moves the character along a spline arc.

Variables:

StartLocation

TargetLocation

ArcHeight

Duration

SplineComponent

ElapsedTime

Functions:

Activate()
Starts the arc movement.

TickTask()
Updates character position along the spline.

OnDestroy()
Cleans up the task.

================================================================================================================

EnemyCharacter

Basic enemy class
