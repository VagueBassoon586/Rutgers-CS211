#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//001: Incorrect argv[] params;
//002: Unable to open and read argv[1];
//003: Unrecognizable directives;

//101: Cannot realloc vars;
//102: Cannot realloc gates;
//103: Cannot malloc INP;
//104: Cannot malloc OUT;
//105: Cannot malloc not;
//106: Cannot malloc and;
//107: Cannot malloc or;
//108: Cannot malloc nand;
//109: Cannot malloc nor;
//110: Cannot malloc xor;
//111: Cannot malloc decoder;
//112: Cannot malloc multiplexer;
//113: Cannot malloc pass;

//201: Cannot fscanf INPUT directive (or INPUT is not present);
//202: Cannot fscanf OUTPUT directive (or OUTPUT is not present);
//203: Cannot fscanf nINP;
//204: Cannot fscanf input variables' name;
//205: Cannot fscanf nOUT;
//206: Cannot fscanf output variables' name;
//207: Cannot fscanf gates' varsIndex;
//208: Cannot fscanf decoders' num;
//209: Cannot fscanf multiplexers' num;

typedef enum {NOT, AND, OR, NAND, NOR, XOR, DECODER, MULTIPLEXER, PASS} gateType;

typedef struct
{
	gateType type;
	int size;
	int *varsIndex;
	int nVar;
} logicgate;

typedef struct
{
	char name[17];
} variable;

variable *vars = NULL;
int numVar = 0;
int capVar = 0;

logicgate *gates = NULL;
int numGate = 0;
int capGate = 0;

void varsSizeCheck(void)
{
	if (numVar >= capVar)
	{
		capVar = (capVar == 0) ? 16 : capVar * 2;
		vars = realloc(vars, capVar * sizeof(variable));
		if (!vars)
			exit(101);
	}
}

void gatesSizeCheck(void)
{
	if (numGate >= capGate)
	{
		capGate = (capGate == 0) ? 16 : capGate * 2;
		gates = realloc(gates, capGate * sizeof(logicgate));
		if (!gates)
			exit(102);
	}
}

int getIndex(const char *name)
{
	for (int i = 0; i < numVar; i++)
		if (strcmp(vars[i].name, name) == 0)
			return i;

	varsSizeCheck();
	strncpy(vars[numVar].name, name, 16);
	vars[numVar].name[16] = '\0';
	numVar++;
	return numVar - 1;
}

void addGate(gateType type, int size, int *varsIndex, int nVar)
{
	gatesSizeCheck();
	gates[numGate].type = type;
	gates[numGate].size = size;
	gates[numGate].varsIndex = varsIndex;
	gates[numGate].nVar = nVar;
	numGate++;
}

void parseINPUT(FILE *circuits, int *nINP, int **INP)
{
	if (fscanf(circuits, " %d", nINP) != 1)
		exit(203);

	*INP = malloc(*nINP * sizeof(int));
	if (!*INP)
		exit(103);

	char name[17];

	for (int i = 0; i < *nINP; i++)
	{
		if (fscanf(circuits, " %16s", name) != 1)
			exit(204);

		int index = getIndex(name);
		(*INP)[i] = index;
	}
}

void parseOUTPUT(FILE *circuits, int *nOUT, int **OUT)
{
	if (fscanf(circuits, " %d", nOUT) != 1)
		exit(205);

	*OUT = malloc(*nOUT * sizeof(int));
	if (!*OUT)
		exit(104);

	char name[17];

	for (int i = 0; i < *nOUT; i++)
	{
		if (fscanf(circuits, " %16s", name) != 1)
			exit(206);

		int index = getIndex(name);
		(*OUT)[i] = index;
	}
}

int readGateParam(FILE *circuits)
{
	char name[17];
	if (fscanf(circuits, " %16s", name) != 1)
		exit(207);

	if (strcmp(name, "0") == 0)
		return -1;
	if (strcmp(name, "1") == 0)
		return -2;
	if (strcmp(name, "_") == 0)
		return -3;

	return getIndex(name);
}

void parseGATE(FILE *circuits)
{
	char token[17];

	while (fscanf(circuits, " %16s", token) == 1)
	{
		if (strcmp(token, "NOT") == 0)
		{
			int *not = malloc(2 * sizeof(int));
			if (!not) 
				exit(105);
			for (int i = 0; i < 2; i++)
				not[i] = readGateParam(circuits);
			addGate(NOT, 0, not, 2);
		}
		else if (strcmp(token, "AND") == 0)
		{
			int *and = malloc(3 * sizeof(int));
			if (!and) 
				exit(106);
			for (int i = 0; i < 3; i++)
				and[i] = readGateParam(circuits);
			addGate(AND, 0, and, 3);
		}
		else if (strcmp(token, "OR") == 0)
		{
			int *or = malloc(3 * sizeof(int));
			if (!or) 
				exit(107);
			for (int i = 0; i < 3; i++)
				or[i] = readGateParam(circuits);
			addGate(OR, 0, or, 3);
		}
		else if (strcmp(token, "NAND") == 0)
		{
			int *nand = malloc(3 * sizeof(int));
			if (!nand) 
				exit(108);
			for (int i = 0; i < 3; i++)
				nand[i] = readGateParam(circuits);
			addGate(NAND, 0, nand, 3);
		}
		else if (strcmp(token, "NOR") == 0)
		{
			int *nor = malloc(3 * sizeof(int));
			if (!nor) 
				exit(109);
			for (int i = 0; i < 3; i++)
				nor[i] = readGateParam(circuits);
			addGate(NOR, 0, nor, 3);
		}
		else if (strcmp(token, "XOR") == 0)
		{
			int *xor = malloc(3 * sizeof(int));
			if (!xor) 
				exit(110);
			for (int i = 0; i < 3; i++)
				xor[i] = readGateParam(circuits);
			addGate(XOR, 0, xor, 3);
		}
		else if (strcmp(token, "DECODER") == 0)
		{
			int nInput;
			if (fscanf(circuits, " %d", &nInput) != 1)
				exit(208);

			int nOutput = 1 << nInput;
			int total = nInput + nOutput;

			int *decoder = malloc(total * sizeof(int));
			if (!decoder) 
				exit(111);

			for (int i = 0; i < total; i++)
				decoder[i] = readGateParam(circuits);

			addGate(DECODER, nInput, decoder, total);
		}
		else if (strcmp(token, "MULTIPLEXER") == 0)
		{
			int nSelectors;
			if (fscanf(circuits, " %d", &nSelectors) != 1)
				exit(209);

			int nInput = 1 << nSelectors;
			int total = nInput + nSelectors + 1;

			int *multiplexer = malloc(total * sizeof(int));
			if (!multiplexer) 
				exit(112);

			for (int i = 0; i < total; i++)
				multiplexer[i] = readGateParam(circuits);

			addGate(MULTIPLEXER, nSelectors, multiplexer, total);
		}
		else if (strcmp(token, "PASS") == 0)
		{
			int *pass = malloc(2 * sizeof(int));
			if (!pass) 
				exit(113);
			for (int i = 0; i < 2; i++)
				pass[i] = readGateParam(circuits);
			addGate(PASS, 0, pass, 2);
		}
		else
			exit(3);
	}
}

int getVal(int *values, int index)
{
	if (index >= 0)
		return values[index];
	else if (index == -1)
		return 0;
	else if (index == -2)
		return 1;
	return 0;
}

void writeVal(int *values, int index, int bit)
{	
	if (index >= 0)
		values[index] = bit;
}

void computeGate(logicgate *gate, int *values)
{
	int *gateVars = gate->varsIndex;
	switch(gate->type)
	{
		case NOT:
		{
			int bit = getVal(values, gateVars[0]);
			writeVal(values, gateVars[1], !bit);
			break;
		}
		case AND:
		{
			int a = getVal(values, gateVars[0]);
			int b = getVal(values, gateVars[1]);
			writeVal(values, gateVars[2], a & b);
			break;
		}
		case OR:
		{
			int a = getVal(values, gateVars[0]);
			int b = getVal(values, gateVars[1]);
			writeVal(values, gateVars[2], a | b);
			break;
		}
		case NAND:
		{
			int a = getVal(values, gateVars[0]);
			int b = getVal(values, gateVars[1]);
			writeVal(values, gateVars[2], !(a & b));
			break;
		}
		case NOR:
		{
			int a = getVal(values, gateVars[0]);
			int b = getVal(values, gateVars[1]);
			writeVal(values, gateVars[2], !(a | b));
			break;
		}
		case XOR:
		{
			int a = getVal(values, gateVars[0]);
			int b = getVal(values, gateVars[1]);
			writeVal(values, gateVars[2], a ^ b);
			break;
		}
		case DECODER:
		{
			int n = gate->size;
			int len = 1 << n;
			int index = 0;
			for (int i = 0; i < n; i++)
				index += getVal(values, gateVars[i]) * (1 << (n - 1 - i));
			for (int i = 0; i < len; i++)
				writeVal(values, gateVars[n + i], 0);
			writeVal(values, gateVars[n + index], 1);
			break;
		}
		case MULTIPLEXER:
		{
			int n = gate->size;
			int len = 1 << n;
			int index = 0;
			for (int i = 0; i < n; i++)
				index += getVal(values, gateVars[len + i]) * (1 << (n - 1 - i));
			writeVal(values, gateVars[n + len], getVal(values, gateVars[index]));
			break;
		}
		case PASS:
		{
			int bit = getVal(values, gateVars[0]);
			writeVal(values, gateVars[1], bit);
			break;
		}
		default:
			break;
	}
}

void truthtable(int nINP, int nOUT, int *INP, int *OUT)
{
	int entries = 1 << nINP;
	int *values = malloc(numVar * sizeof(int));
	if (!values)
		exit(114);
	int *frequency = malloc(nINP * sizeof(int));
	if (!frequency)
		exit(115);
	int *boolSwitcher = malloc(nINP * sizeof(int));
	if (!boolSwitcher)
		exit(116);
	int *bits = malloc(nINP * sizeof(int));
	if (!bits)
		exit(117);
	for (int i = 0; i < nINP; i++)
	{
		entries = (int) entries / 2;
		frequency[i] = entries;
		boolSwitcher[i] = frequency[i];
		bits[i] = 0;
	}
	unsigned long long rows = 1ULL << nINP;
	for (unsigned long long row = 0; row < rows; row++)
	{
		for (int v = 0; v < numVar; v++)
			values[v] = 0;

		for (int i = 0; i < nINP; i++)
		{
			if (boolSwitcher[i] == 0)
			{
				bits[i] = ((bits[i] == 0) ? 1 : 0);
				boolSwitcher[i] = frequency[i];
			}
			values[INP[i]] = bits[i];
			boolSwitcher[i]--;
		}
		
		for (int gate = 0; gate < numGate; gate++)
			computeGate(&gates[gate], values);

		for (int i = 0; i < nINP; i++)
		{
			int val = values[INP[i]];
			printf("%d ", val);
		}

		printf("|");

		for (int i = 0; i < nOUT; i++)
		{
			int val = values[OUT[i]];
			printf(" %d", val);
		}
		printf("\n");
	}
    free(bits);
    free(boolSwitcher);
    free(frequency);
    free(values);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		exit(1);

	FILE *circuits = fopen(argv[1], "r");
	if (!circuits)
		exit(2);

	char token[17];

	if (fscanf(circuits, " %16s", token) != 1 || strcmp(token, "INPUT") != 0)
		exit(201);

	int nINP;
	int *INP;
	parseINPUT(circuits, &nINP, &INP);

	if (fscanf(circuits, " %16s", token) != 1 || strcmp(token, "OUTPUT") != 0)
		exit(202);

	int nOUT;
	int *OUT;
	parseOUTPUT(circuits, &nOUT, &OUT);

	parseGATE(circuits);

	fclose(circuits);

	truthtable(nINP, nOUT, INP, OUT);

	for (int g = 0; g < numGate; g++)
		free(gates[g].varsIndex);
	free(gates);
	free(INP);
	free(OUT);
	free(vars);

	return EXIT_SUCCESS;
}
