# 25-26-DyAA-p05-MS-CFLP-CI

Multi-Source Capacitated Facility Location Problem with Customer Incompatibilities (MS-CFLP-CI)

## Problem description

The **Capacitated Facility Location Problem** (MS-CFLP-CI) is a variant of the classic facility location problem. In this scenario, we must decide which warehouses to open and how to assign stores to satisfy their demand at the lowest possible cost. This model introduces two key features:

1. Multi-source: Each store can receive their goods in multiple open warehouses simultaneously to complete their total demand.

2. Incompatibilities: There are pairs of stores that cannot be served by the same warehouse.

The objective of the MS-CFLP-CI is to minimize the sum of the fixed costs of opening warehouses and the variable transportation costs.

## Instance description

See `docs/instance-MS-CFPL-CI.md`

## Available instances

See `./Instances_MS-CFLP-CI`. **Check if its need to unzip some instance**. Probably wlp[16..20].dzn are compressed.

## Available algortihms

> Greedy
> GRASP (phase 1: construction, phase 2: local search) Local search movements: [shift, swap stores, swap warehouses and ban incompatibilities]


## Solution description

See `docs/solution-MS-CFPL-CI.md`

## Run code

1. mkdir build
2. cd build
3. cmake ..
4. make
5. /MS-CFPL-CI-solver -h|--help

## Other information

This code was developed by 
* [Cristhian Adoney Cruz Delgado](https://github.com/CristhianCruzDelgado)

Contact emails 
* <alu0101648293@ull.edu.es>

This code was designed for Design and Analysis of Algorithms, \
signature of the Computer Engineering degree, \
that is studied at the Univeridad de La Laguna.

_24 Mar 2026_