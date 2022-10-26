# MLS-5

A 5-loop, MIDI-Controllable Switcher

Features include:
 - MIDI control of any or all loops simultaneously
 - Local control of individual loops
 - Indicator LED's for enabled loops

The code, layouts, build instructions, and other documentation are provided to the DIY community for personal, non-professional use by Brian Thornock, copyright 2019

There's also an additional [article](https://scientificguitarist.wixsite.com/home/mls-5) and a [demonstartion video](https://www.youtube.com/watch?v=fT1JugYtmUw).

![Fig. X: Box Jack](/docs/assets/images/MLS_5.jpg)

## Overview

The MLS-5 came about as a way to add more loops to my Boss MS-3. The MS-3 is programmed to send MIDI changes to the MLS-5 on channel 2 and allows the MS-3 to have a total of 7 loops that can be used. I admit it, I'm a dirt addict. In addition to responding to MIDI patch changes, the local switches can also be used to switch individual loops on and of. When controlling via MIDI, any combination of loops can be enabled with a single patch change on the MS-3.

## How it Works

Very briefly, it works using an Arduino nano that reads its serial data buffer to see if it has any received MIDI messages. If so, it enables the appropriate relays. If not, it checks the onboard switches to see if they have been pressed. The onboard switches are momentary, Normally-open SPST switches (nice and quiet) and all loops are mono. The change to stereo would be a fairly significant change, as it would require more relays and a larger board. The signal path is all true bypass with no buffering, but the addition of an input or output buffer would be relatively trivial.

## Bill of Materials

The BOM below is the list of parts that I used for mine along with quantities. I got most of my stuff from Tayda, though Mouser was necessary for a few things, like the relays, optocoupler, and MIDI jack. I'll provide part numbers where possible.

| **Part**                  | **Qty.** | **Part # (if available)** | **Supplier/Notes**                   |
| ------------------------- | -------- | ------------------------- | ------------------------------------ |
| 6N138 Optocoupler         | 1        | [859-6N138M](https://www.mouser.com/ProductDetail/Lite-On/6N138M?qs=PByDJ0nQNwr%252BImBkghBeaw%3D%3D) | Mouser |
| MIDI Jack                 | 1        | [161-0005-E](https://www.mouser.com/ProductDetail/Kobiconn/161-0005-E?qs=xCMk%252BIHWTZN7lno9NfzG8g%3D%3D) | Mouser |
| Relay                     | 5        | [655-1-1393793-2](https://www.mouser.com/ProductDetail/TE-Connectivity-PB/V23105A5475A201?qs=CUfic%2FGsH5A1QhddMEag5A%3D%3D),[A-168](https://www.taydaelectronics.com/na-5w-k-mini-relay-dpdt-5vdc-1a.html) | Mouser, Tayda |
| BC548                     | 5        | [A-141](https://www.taydaelectronics.com/bc548-transistor-npn-30v-0-1a.html) | Tayda |
| DC Power Jack             | 1        | [A-2237](https://www.taydaelectronics.com/dc-power-jack-2-1mm-enclosed-frame-with-switch.html) | Tayda |
| 1/4" Phone Box jack                  | 12       | [A-1121](https://www.taydaelectronics.com/6-35mm-1-4-stereo-phone-jack.html) | Tayda |
| LED                       | 5        |                           | Tayda                                |
| LED holder                | 5        |                           | Tayda                                |
| SPST, NO Momentary Switch | 5        | [A-2371](https://www.taydaelectronics.com/spst-momentary-soft-touch-short-shaft-push-button-stomp-foots-pedal-switch.html) | Tayda |
| Arduino Nano              | 1        |                           | Tayda, Aliexpress                    |
| 1N4148                    | 6        | [A-157](https://www.taydaelectronics.com/1n4148-switching-signal-diode.html) | Tayda |
| LM7805                    | 1        | [A-179](https://www.taydaelectronics.com/lm7805-l7805-7805-voltage-regulator-ic-5v-1-5a.html) | Tayda |
| 78L05                     | 1        | [A-176](https://www.taydaelectronics.com/l78l05acz-l78l05-78l05-5-volts-100ma-voltage-regulator-ic.html) | Tayda |
| 1N5817                    | 1        | [A-159](https://www.taydaelectronics.com/1n5817-diode-schottky-1a-20v.html) | Tayda |
| 2 Switch DIP Switch       | 1        | [A-5016](https://www.taydaelectronics.com/dip-switch-2-positions-gold-plated-contacts-top-actuated.html) | Tayda |
| Capacitor: 47 uF electro  | 2        |                           | Tayda                                |
| Resistor: 220R            | 1        |                           | Tayda                                |
| Resistor: 470R            | 1        |                           | Tayda                                |
| Resistor: 4k7             | 5        |                           | Tayda                                |
| Resistor: 10k             | 6        |                           | Tayda                                |
| Resistor: 22k             | 5        |                           | Tayda                                |
| Resistor: 100k            | 5        |                           | Tayda                                |
| Resistor: Matched for LED | 5        |                           | Tayda                                |
| Resistor: 0R47            | 1        |                           | Tayda, but it's really just a jumper |
| 1032l Enclosure           | 1        | [A-5853](https://www.taydaelectronics.com/1032l-style-aluminum-diecast-enclosure.html) | Tayda |

## Schematic

[Go to the schematics folder](/schematics/) to learn more.

## Programming the Arduino

[Go to the firmware folder](/firmware/) to learn more.

## Build Notes

The MLS-5 is built to fit into a 1023L-type enclosure. I'm not going to lie, it's a tight fit and uses a bunch of wire. However, I have learned several lessons in building one that will make your life much easier. These are discussed individually below.

### Jacks/Plugs

In order to ensure that all the jacks will fit and to provide a surface for mounting the PCB, use the square cross section, box-type jacks (See Fig. X). Mount the first one very close to the edge of the box, as close to what will be the top surface of the box as possible. I spaced mine every 0.65” and it worked out quite well (see Fig. Y). With this spacing, standard sized straight and angled plugs will work. Pancake jacks WILL NOT work. I discovered the Rockboard flat patch cables, and the size of the plug works really well with the dimensions of the enclosure.

Additionally, the box jacks are stereo with two switch contacts, so make sure the get rid of the extra terminals by bending them back and forth repeatedly until they snap off. Just to make things easy, the terminal on the diagonal corner is ground. If this is at top left, then the terminal on the bottom is the tip terminal (see Fig. A).

![Fig. X: Box Jack](/docs/assets/images/63_Jack_Socket.jpg)
*Fig. X: Box Jack*

![Fig. Y: Jack layout](/docs/assets/images/Jack_Wiring.jpg)
*Fig. Y: Jack layout*

![Fig. A: Box Jack Terminals](/docs/assets/images/Box_Jack_Terminals.png)
*Fig. A: Box Jack Terminals (Viewed from Terminal Side of Jack)*

### Wire Gauge and Length

In this project, most of the wires are only carrying DC voltage. For example, the switches, LED's, and power jack are only carrying DC, so wire gauge is not really a concern. I recommend 24 AWG stranded wire for pretty much everything, though. I used solid core for the switches and it made things very difficult.

As for wire length, make sure that you have enough wire to comfortably get from point A to point B. I had to re-wire all of the jacks because I made the original wire runs too short to allow for soldering to the board with it partially removed. There's plenty of space in the box for a little extra wire. If I were doing this again, I would probably get the pre-wired LED's from someplace like BLMS.

### Jack Installation Order

This might not seem like that big of a deal, but it just helps with frustration. Install the DC power jack first, then install the box jacks. The MIDI jack comes last. I used #6 machine screws and nuts to keep it in. I recommend the pan head and not flat head. Trust me.

### PCB's

I opted for a pair of stacked PCB's. The DIY layout creator file included in the zip file is made for this. Both PCB's can come from a single 70x90 mm perf board. It allows for 1 or two rows along which you can cut and still have all the space. If you choose to go this way, here are a few items to keep in mind:

* Use stackable headers on the bottom board and pin headers on the top board. The square pads in the DIYLC file indicates where I put headers. As seen below (see Fig. Z) I used continuous sections of stackable headers, but I pulled out the pins from the unused locations to make things cleaner
* Using the header configuration can help with alignment, but the first couple of fittings can be a little funky, so maybe put it together a few times before soldering in the wires.
* I pulled the pins out of the pin headers and used the plastic remainder to shim up the stackable headers to get just a little more room between the two boards. This allowed me to use a heatsink on the TO-220 voltage regulator. However, I don't think this is necessary. This amount of space is insufficient to socket the Arduino, which I probably would have preferred, but c'est la vie. I ended up soldering one in, frying it, and having to cut it out and then finally get a new one in.
* If doing stackable boards, do NOT use sockets for the relays. I tried and was 1-2 mm too tall to fit in the enclosure. Once I put in the relays flush to the board, it stays firmly in place just due to the placement of the lid.
* Overall, this arrangement works fine, but a single PCB would offer advantages


![Fig. Z: ](/docs/assets/images/Stacked_PCBs.jpg)
*Fig. Z: Stacked PCB Arrangement Soldered and Ready to Put In. Yeah, It's a Bit of a Mess*

Using a single PCB would have some advantages, such as more head room for socketing parts and smaller wire density. However, it would require some very fiddly procedure with the LED's and switch wiring. I might work up a PCB layout for one, but don't expect it immediately. The sheer size of it would make placement in the enclosure a little more difficult as well, I feel, but it's totally doable. Just another lesson learned. Maybe effectslayouts.blogspot.com can work up a sweet layout (hint, hint :))!

### MIDI Channels and Mapping

One thing that caused me some grief is the fact that MIDI in code is standard 0-indexed, whereas MIDI interfaces, such as the Boss MS-3, index from 1. This means that a digital 0 is a 1 to the interface device. With that in mind, here is a handy table I've put together for the various configurations and what the MIDI patch numbers are. Note that I am number the loops from right to left, since that is how the signal flows. Also, if you want to change the MIDI channel, it is a straightforward procedure of modifying the Arduino code in the .zip file. Note that in the table below, there are some combinations marked with an *. These indicate duplicates where behavior may not be well defined due to the fact that 1-indexing results in some non-unique combinations of numbers. The functionality here is hit and miss. Testing on my one device showed two different possible states that could be gotten into using patch change number 14. Basically, any combination that uses both channels 1 and 2 will have the potential for confusion. So user beware!

**All Patches on MIDI Channel 2!!!**

| **Active Loops** | **MIDI Patch Number** |
| ---------------- | --------------------- |
| 1                | 2                     |
| 2                | 3                     |
| 3                | 5                     |
| 4                | 9                     |
| 5                | 17*                   |
| 1,2              | 5*                    |
| 1,3              | 7                     |
| 1,4              | 11                    |
| 1,5              | 19                    |
| 2,3              | 8                     |
| 2,4              | 12                    |
| 2,5              | 20                    |
| 3,4              | 14*                   |
| 3,5              | 22*                   |
| 4,5              | 26                    |
| 1,2,3            | 10                    |
| 1,2,4            | 14*                   |
| 1,2,5            | 22*                   |
| 1,3,4            | 16                    |
| 1,3,5            | 24                    |
| 1,4,5            | 28                    |
| 1,2,3,4          | 19                    |
| 1,2,3,5          | 27                    |
| 1,2,4,5          | 31*                   |
| 1,2,3,4,5        | 36                    |
| 2,3,4            | 17*                   |
| 2,3,5            | 25                    |
| 2,4,5            | 29                    |
| 2,3,4,5          | 34                    |
| 3,4,5            | 31*                   |

*Table 1: MIDI Patch Assignements*

### Power Consumption

Of course, on a pedalboard, power consumption is always a concern. With this, the power consumption is directly related to how many relays are active at any given time. The relays used here are non- latching, which means that they consume power the entire time they are on. While latching were a possibility, there would have been more wiring necessary and there wouldn't have been any real way of having the LED's on as well; the Arduino doesn't have enough pins for all this. The relays I used consume about 25-30 mA each when energized. On top of that, the Arduino and such will consume about another 20-30 mA. This means that a dedicated 200 mA supply should be sufficient to run all relays at all times. If, like me, you don't intend to stack 5 overdrives, then a standard 100-150 mA supply is fine.

### Switch Debouncing Time

In the code found in the .zip file, the switch debounce time is perhaps a little generous. I have found that this results in needing to hold the onboard switches just a hair longer than a quick tap. If this annoys you, you can reduce the debounce delay in the code and make it a little more responsive, but note that doing so may result in a higher occurrence of false triggers. I haven't found any to date, but just note that it's a possibility.

### Decorative Switch Nuts

I chose to put them on for some pizzazz. Well, the momentary switches have a different thread tham the 3PDT switches they are made for. I sort of made them work, but were I doing it again, I would leave them off.

## In Closing

Thanks for stopping by and for your interest! I hope you have as much fun with yours as I have with mine. Unfortunately, the only support I can offer for this DIY project is on the DIYstompboxes and FreeStompboxes forums. In addition, the good people there are a wealth of info. Best of luck!
