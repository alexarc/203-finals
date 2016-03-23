# FX Trading Bot
## Overview

The goal of this assignment is to simulate an arbitration program that would be connected to several exchanges and post orders in order to turn risk-less profits.

## Tier 1

Read from a text file a list of events, where an events contains:
 - A timestamp,
 - The name of the exchange,
 - A currency pair,
 - The rate at which the pair trades

Output a list of arbitration possibilities and the time (the timestamp) at which they present themselves.

## Tier 2

To add some realism to the situation, add to the events:
 - Open/Close events who mark availability of a given exchange
 - Market depth for each rate update

When outputting orders, make sure the execution is optimal - better margins must be executed first.

## Tier 3

Make the program react in real-time to the events. For that, use [pipes](http://ss64.com/nt/syntax-redirection.html) and a second program that delays the transmission of the information until the time matches the simulation time.
