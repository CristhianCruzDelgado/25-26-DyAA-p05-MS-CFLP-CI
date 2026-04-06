# Documentation of class SolutionMSCFLPCI

### Description

The `class SolutionMSCFLPCI` represents all of the components of the **Multi-Supplier Capacitated Facility Location Problem with Client Incompatibilities (MS-CFLP-CI)**. It encapsulates a complete problem solution, storing all output data generated to deciding which warehouses to open and how each store is assigned to them

The goal of the MS-CFLP-CI is to minimize the total cost, which includes both the fixed costs of opening warehouses and supply costs between warehouses and clients. At the same time, the goods of every store must be fully satisfied, and all incompatibility constraints between clients must be respected

| Attribute            | Type                  | Description                                                                                   |
|----------------------|-----------------------|-----------------------------------------------------------------------------------------------|
| instance_data_       | InstanceMS_CFLP_CI*   | Pointer to the problem instance containing all input data                                     |
| warehouses_assigned_ | vector<short>         | Indices of warehouses selected to be open in the solution                                     |
| assignments_         | vector<vector<short>> | Binary matrix where assignments_[i][j] = 1 if store i is assigned to warehouse j              |
| good_supplied_       | vector<vector<float>> | Matrix where good_supplied_[i][j] is the fraction of goods of store i supplied by warehouse j |
| residual_capacity_   | vector<short>         | Remaining capacity of each warehouse after assigning goods                                    |
| residual_goods_      | vector<short>         | Remaining unsatisfied demand of each store                                                    |
| time_                | double                | Execution time required to compute the solution                                               |
| fixed_costs_sum_     | float                 | Total fixed cost of all opened warehouses                                                     |
| supply_costs_sum_    | float                 | Total transportation cost for supplying goods to stores                                       |
| objective_value_     | float                 | Total objective value (fixed costs plus supply costs)                                         |

### Descripción de la solución

Atendiendo al enunciado de la práctica ...

| Campo           | Tipo              | Descripción |
|-----------------|-------------------|-------------|
| **Estructuras primarias** |||
| open            | boolean[m]        | open[j] = true si la instalación j está abierta (yj = 1) |
| x               | double[n][m]      | x[i][j]: fracción de la demanda di servida por j (xij) |
| w               | boolean[n][m]     | w[i][j] = true si el cliente i es servido por j (wij) |
| **Estructuras auxiliares** |||
| residualCap     | double[m]         | Capacidad libre de j: sj − Σᵢ∈I di · xij |
| clientsOf       | List[m]           | Clientes servidos (parcial o totalmente) por j |
| facilitiesOf    | List[n]           | Instalaciones que sirven al cliente i |
| incompCount     | int[n][m]         | Número de clientes incompatibles con i asignados a j |
| **Coste descompuesto** |||
| fixedCost       | double            | Σⱼ∈J fj · yj |
| transportCost   | double            | Σᵢ∈I Σⱼ∈J cij · di · xij |
| totalCost       | double            | fixedCost + transportCost |

### Resultados

Aunque se dispondrán de varias tablas para representar la solución esta será la que medirá el valor objetivo final. En función de ella se puede medir la calidad de cada solución ofrecida por los distintos algoritmos que generan las distintas soluciones.

Tabla 9: Resultados
| Instancia | m     | n     | Greedy (value, time) | GRASP  (value, time) |
|-----------|-------|-------|----------------------|----------------------|
| wlp01     | 50    | 115   |    37004, ~0.0025    |           ?          |
| wlp02     | 100   | 253   |    70313, ~0.03      |           ?          |        
| wlp03     | 150   | 345   |    86477, ~0.07      |           ?          |
| wlp04     | 200   | 479   |   103105, ~0.15      |           ?          |
| wlp05     | 250   | 601   |   129380, ~0.4       |           ?          |
| wlp06     | 300   | 705   |   151083, ~0.8       |           ?          | 
| wlp07     | 400   | 1012  |   213769, ~2.5       |           ?          | 
| wlp08     | 500   | 1277  |   248919, ~4.8       |           ?          | 
| wlp09     | 600   | 1483  |    
| wlp10     | 700   | 1733  |    
| wlp11     | 800   | 2020  |    
| wlp12     | 900   | 2159  |    
| wlp13     | 1000  | 2305  |    
| wlp14     | 1200  | 2927  |    
| wlp15     | 1400  | 3445  |    
| wlp16     | 1600  | 4067  |    
| wlp17     | 1800  | 4373  |    
| wlp18     | 2000  | 4908  |    
| wlp19     | 2500  | 5882  |    
| wlp20     | 3000  | 7800  |    

### Representación de la solución

> Tabla 4: Solución a las instalaciones

| ID | Estados | Costo fijo (fj) |
|----|---------|-----------------|
| .. | ...     | ...             |

> Tabla 5: Información de asignación de clientes

| ID | ID | Demanda (di) | % |
|----|----|--------------|---|
| .. | .. | ...          | . |

> Tabla 6.1: Verificación de restricciones

| ID | Carga total      | Capacidad (sj)   | Estado   |
|----|------------------|------------------|----------|
| .. | ...              | ...              | ...      |

> Tabla 6.2: Verificación de incompatibilidades

| Cliente primero  | Cliente segundo  | Estado   |
|------------------|------------------|----------|
| ...              | ...              | ...      |

> Tabla 7: Solución a la asignación de clientes

| ID | ID | Demanda (di * %) | Capacidad (sj)   | Subtotal |
|----|----|------------------|------------------|----------|
| .. | .. | ...              | ...              | ...      |

> Tabla 8: Valores objetivos

| Valores objetivos     | Coste       |
|-----------------------|-------------|
| Costo fijo (Tf)       | ...         |
| Costo transporte (Tt) | ...         |
| Total (T)             | T = Tf + Tt |
| Time (seconds)        | ...         |

> Tabla 9: Resultados

|----------------|---------------------|---------------------|----------------|
| wlp 1 .. 20    | clientes **m**      | instalaciones **n** | resultados ... |
