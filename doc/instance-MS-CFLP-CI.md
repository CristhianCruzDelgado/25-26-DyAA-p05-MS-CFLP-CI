# Documentation of class InstanceMSCFPLCI

### Description

The `class InstanceMSCFLPCI` represents all of the components of the **Multi-Supplier Capacitated Facility Location Problem with Client Incompatibilities (MS-CFLP-CI)**. It encapsulates a complete problem instance, storing all input data required to model warehouses, stores, supply costs, and pairwise store incompatibilities.

### Attributes

| Attribute            | Type                  | Description                                                                       |
|----------------------|-----------------------|-----------------------------------------------------------------------------------|
| capacity_            | vector<short>         | Capacity of each warehouse                                                        |
| fixed_cost_          | vector<short>         | Fixed opening cost of each warehouse                                              |
| goods_               | vector<short>         | Demand (goods required) of each store                                             |
| supply_cost_         | vector<vector<short>> | Matrix where supply_cost_[i][j] is the cost of supplying store j from warehouse i |
| incompatible_pairs_  | vector<ShortPair>     | List of store pairs ⟨i1, i2⟩ that cannot be assigned to the same warehouse        |

### Strategy Pattern

The class implements the Strategy pattern through the Algorithm* pointer. The concrete solving algorithm is decoupled from the instance data: setAlgorithm(Algorithm*) injects any Algorithm implementation at runtime, and solve() delegates execution to it. This allows switching between, for example, a greedy heuristic, a local search, or an exact solver without modifying this class.

### Descripción de la instancia

Atendiendo al enunciado de la práctica ...

> "Clientes (I): Un conjunto de n clientes, donde cada cliente i tiene una demanda específica di."

El número de clientes n se ha codificado en el número de filas de la matriz `supply cost` y
la demanda para cada cliente se almacena en el vector `goods`, que también es de tamaño n.

> "Instalaciones (J): Un conjunto de m ubicaciones potenciales para abrir instalaciones, cada una tiene un coste fijo de apertura fj y una capacidad máxima de suministro sj"

El número de ubicaciones m para instalaciones se ha codificado en el número de columnas de la matriz `supply cost`,
el coste fijo de apertura se almacena en el vector `fixed cost` y la capacidad máxima en el vector `capacity`, ambos de tamaño m.

> "Costos de Transporte: Un coste cij por cada unidad de demanda enviada desde la instalación j al cliente i."

El coste de transporte se almacena en la matriz `supply cost`, de manera que para cada ubicación de una instalación existe un coste de transporte hasta un cliente.

> "Restricciones de Incompatibilidad (C): Un conjunto de pares de clientes ⟨i1, i2⟩ que no pueden ser abastecidos por la misma instalación bajo ninguna circunstancia."

Las restricciones suponen que dos clientes no pueden ser demandantes de una misma instalación y por ello se almacenan clientes incompatibles en el vector `incompatible pairs`.

### Representación de la instancia

> Tabla 1.1: Información de instalaciones
> Tabla 1.2: Información de clientes

| ID | Capacidad (sj) | Coste Fijo (fj) | 
|----|----------------|-----------------|
| .. | ...            | ...             |

| ID | Demanda (di) | Incompatibilidades |
|----|--------------|--------------------|
| .. | ...          | ...                |

> Tabla 2: Matriz de costes unitarios de transporte (cij)

|                     | instalaciones 1 .. m |
|---------------------|----------------------|
| **clientes 1 .. n** | ...                  |
