// --------------------------------------------------------------------------------
// Name: Seth Niefield
// Class: SET-151-001 // Final Project
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Includes – built-in libraries of functions
// --------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------
const long ARRAY_SIZE = 5; // This is just an example - you can delete this line

// --------------------------------------------------------------------------------
// User Defined Types (UDT)
// --------------------------------------------------------------------------------

typedef struct
{
	char strDateRecorded[15];
	char strState[15];
	char strCounty[15];
	char strRace[20];
	int intResidents;
	int intIncome;
} udtSurvey;

// --------------------------------------------------------------------------------
// Prototypes
// --------------------------------------------------------------------------------

void New_Survey(udtSurvey** pudtHead);
void Get_Validate_Date(udtSurvey* udtSurvey);
void Get_Validate_State(udtSurvey* udtSurvey);
void Get_Validate_County(udtSurvey* udtSurvey);
void Get_Validate_Race(udtSurvey* udtSurvey);
void Get_Validate_Residents(udtSurvey* udtSurvey);
void Get_Validate_Income(udtSurvey* udtSurvey);
void strCopy(char strDestination[], char strSource[]);
int OpenFileRead(char strFileName[], FILE** ppfilInput);
int OpenFileAppend(char strFileName[], FILE** ppfilInput);
void PrintToFile(FILE** ppfilOutput, udtSurvey* pudtHead);
void ClearString(char strDestination[]);
void TotalHouseholdsSurveyed(char strFileName[]);
void TotalHouseholdsSurveyedPerCounty(udtSurvey* udtSurveys, int intLines);
void TotalHouseholdsSurveyedPerRace(udtSurvey* udtSurveys, int intLines);
void AverageHouseholdIncome(udtSurvey* udtSurveys, int intLines);
void AverageHouseholdIncomeByCountyAndState(udtSurvey* udtSurveys, int intLines);
void AverageHouseholdIncomeByRace(udtSurvey* udtSurveys, int intLines);
void PercentageBelowPoverty(udtSurvey* udtSurveys, int intLines);
void PercentageBelowPovertyByCountyAndState(udtSurvey* udtSurveys, int intLinest);
void PercentageBelowPovertyByRace(udtSurvey* udtSurveys, int intLines);
int PopulateSurveyArray(udtSurvey* udtSurveys, char strFileName[]);
void AddSurveyToArray(char strAddress[], udtSurvey* pudtSurvey);
int Length(char strSource[]);
int strCopyTilComma(char strDestination[], char strSource[], int intIndex);
void PrintOptions();

//----------------------------------------
// Name: main
// Abstract: Initiates Program
//----------------------------------------

int main()
{
	printf("Welcome to Seth's Census Buraue!\n\n");
	int intResult = 0;
	char strFileName[30] = "FinalProject_SN.csv";
	//udtSurvey* ppudtSurveys = (udtSurvey*)malloc(30 * sizeof(udtSurvey)); //Array of 30
	while (intResult != 3)
	{
		printf("\n\nPress one of the following : \n1 for Wirte Mode\n2 for Display Mode\n3 to Exit Program\nEnter: ");
		scanf("%d", &intResult);
		if (intResult >= 1 && intResult <= 3)
		{
			udtSurvey* ppudtSurveys = (udtSurvey*)malloc(GetEntries(strFileName) * sizeof(udtSurvey));
			switch (intResult)
			{
			case 1: {
				udtSurvey* pudtHead = NULL;
				int intResult = 0;
				FILE* pfilInput = NULL;
				New_Survey(&pudtHead);
				intResult = OpenFileAppend(strFileName, &pfilInput);
				if (intResult == 1)
				{
					if (pudtHead != NULL) {
						PrintToFile(&pfilInput, pudtHead);
					}
				}
				free(pudtHead);
			}
				  break;
			case 2: {
				int intLines = PopulateSurveyArray(ppudtSurveys, strFileName);
				char strResult[50] = "";
				char chrResult;
				int intValid = 0;
				
				//OPTIONS: a,b,c,d,e,f,g,h,i
				PrintOptions();

				while (intValid == 0)
				{
					printf("Enter: ");
					scanf(" %s", &strResult);
					int intLength = Length(strResult);
					if ( intLength == 1)
					{
						chrResult = strResult[0];
						if (chrResult >= 'a' && chrResult <= 'i')
						{
							intValid = 1;
							printf("\n");
						}
						else if (chrResult >= 'A' && chrResult <= 'I')
						{
							intValid = 1;
							printf("\n");
						}
						else
						{
							printf("Invalid entry.\n\n");
							while (getchar() != '\n');
						}
					}
					else
					{
						printf("Invalid entry.\n\n");
						ClearString(strResult);
					}
				}
				switch (chrResult)
				{
				case 'a':
				case 'A': TotalHouseholdsSurveyed(strFileName); break;
				case 'b':
				case 'B': TotalHouseholdsSurveyedPerCounty(ppudtSurveys, intLines); break;
				case 'c':
				case 'C': TotalHouseholdsSurveyedPerRace(ppudtSurveys, intLines); break;
				case 'd':
				case 'D': AverageHouseholdIncome(ppudtSurveys, intLines); break;
				case 'e':
				case 'E': AverageHouseholdIncomeByCountyAndState(ppudtSurveys, intLines); break;
				case 'f':
				case 'F': AverageHouseholdIncomeByRace(ppudtSurveys, intLines); break;
				case 'g':
				case 'G': PercentageBelowPoverty(ppudtSurveys, intLines); break;
				case 'h':
				case 'H': PercentageBelowPovertyByCountyAndState(ppudtSurveys, intLines); break;
				case 'i':
				case 'I': PercentageBelowPovertyByRace(ppudtSurveys, intLines); break;
				}
				break;
			}
			case 3:
				printf("Goodbye.");
				break;
			default: printf("Please select 1, 2, or 3.\n\n");
				break;
			}
			free(ppudtSurveys);
			
		}
		else
		{
			printf("Please select 1, 2, or 3.\n\n");
			while (getchar() != '\n');
		}
	}
	
}


// --------------------------------------------------------------------------------
// Name: New_Survey
// Abstract: Collects inputs to create a new survey
// --------------------------------------------------------------------------------
void New_Survey(udtSurvey** pudtHead)
{

	udtSurvey* pudtNewSurvey = (udtSurvey*)malloc(sizeof(udtSurvey));
	if (pudtNewSurvey == NULL) {
		printf("Malloc failed. No New survey record was created.");
		return;
	}
	else {
		Get_Validate_Date(pudtNewSurvey);
		Get_Validate_State(pudtNewSurvey);
		Get_Validate_County(pudtNewSurvey);
		Get_Validate_Race(pudtNewSurvey);
		Get_Validate_Residents(pudtNewSurvey);
		Get_Validate_Income(pudtNewSurvey);

		*pudtHead = pudtNewSurvey;
	}
}

// --------------------------------------------------------------------------------
// Name: Get_Validate_Date
// Abstract: Gets and Validates Date
// --------------------------------------------------------------------------------
void Get_Validate_Date(udtSurvey* udtSurvey)
{
	int intValid = 0;
	char strDate[15] = "";

	while (intValid == 0)
	{
		printf("Enter date of entry (xx/xx/xxxx): ");
		scanf("%s", strDate);

		int day = 0;
		int month = 0;
		int year = 0;

		if (sscanf(strDate, "%2d/%2d/%4d", &month, &day, &year) == 3)
		{
			if (month >= 1 && month <= 12 && day >= 1 && day <= 31 && year >= 1900 && year <= 2024)
			{
				strCopy(udtSurvey->strDateRecorded, strDate);
				intValid = 1;
			}
			else
			{
				printf("Invalid date. Please enter a valid date (format: xx/xx/xxxx).\n");
			}
		}
		else
		{
			printf("Invalid format. Please use the format xx/xx/xxxx.\n");
		}
	}
}

// --------------------------------------------------------------------------------
// Name: Get_Validate_State
// Abstract: Gets and Validates State
// --------------------------------------------------------------------------------
void Get_Validate_State(udtSurvey* udtSurvey)
{
	int intValid = 0;
	int intState = 0;
	while (intValid == 0)
	{
		printf("Enter a state -- 1 for Ohio | 2 for Kentucky: ");
		scanf("%d", &intState);
		if (intState == 1)
		{
			strCopy(udtSurvey->strState, "Ohio");
			intValid = 1;
		}
		else if (intState == 2)
		{
			strCopy(udtSurvey->strState, "Kentucky");
			intValid = 1;
		}
		else
		{
			printf("Invalid entry.\n");
			while (getchar() != '\n');
		}
	}
}

// --------------------------------------------------------------------------------
// Name: Get_Validate_County
// Abstract: Gets and Validates County
// --------------------------------------------------------------------------------
void Get_Validate_County(udtSurvey* udtSurvey)
{
	int intValid = 0;
	int intCounty = 0;
	char strState[10] = "";
	strCopy(strState, udtSurvey->strState);
	while (intValid == 0)
	{
		if (strcmp(strState, "Ohio") == 0)
		{
			printf("Enter a county -- 1 for Hamilton | 2 for Butler: ");
			scanf("%d", &intCounty);
			if (intCounty == 1 || intCounty == 2)
			{
				switch (intCounty)
				{
				case 1: strCopy(udtSurvey->strCounty, "Hamilton");
					intValid = 1;
					break;
				case 2: strCopy(udtSurvey->strCounty, "Butler");
					intValid = 1;
					break;
				default: printf("Incorrect Entry.\n");
					break;
				}
			}
			else
			{
				printf("Invalid entry.\n");
				while (getchar() != '\n');
			}
		}
		else if (strcmp(strState, "Kentucky") == 0)
		{
			printf("Enter a county -- 1 for Boone | 2 for Kenton: ");
			scanf("%d", &intCounty);
			if (intCounty == 1 || intCounty == 2)
			{
				switch (intCounty)
				{
				case 1: strCopy(udtSurvey->strCounty, "Boone");
					intValid = 1;
					break;
				case 2: strCopy(udtSurvey->strCounty, "Kenton");
					intValid = 1;
					break;
				default: printf("Invalid Entry.\n");
					break;
				}
			}
			else
			{
				printf("Invalid entry.\n");
				while (getchar() != '\n');
			}
		}
	}
}

// --------------------------------------------------------------------------------
// Name: Get_Validate_Race
// Abstract: Gets and Validates Race
// --------------------------------------------------------------------------------
void Get_Validate_Race(udtSurvey* udtSurvey)
{
	int intValid = 0;
	int intRace = 0;
	while (intValid == 0)
	{
		printf("1 for Caucasian | 2 for African American\n3 for Hispanic | 4 for Asian | 5 for Other\nEnter your race: ");
		scanf("%d", &intRace);
		if (intRace >= 1 && intRace <= 5)
		{
			switch (intRace)
			{
			case 1: strCopy(udtSurvey->strRace, "Caucasian");
				intValid = 1;
				break;
			case 2: strCopy(udtSurvey->strRace, "African American");
				intValid = 1;
				break;
			case 3: strCopy(udtSurvey->strRace, "Hispanic");
				intValid = 1;
				break;
			case 4: strCopy(udtSurvey->strRace, "Asian");
				intValid = 1;
				break;
			case 5: strCopy(udtSurvey->strRace, "Other");
				intValid = 1;
				break;
			default: printf("Invalid Entry.\n");
				break;
			}
		}
		else
		{
			printf("Invalid entry.\n");
			while (getchar() != '\n');
		}

	}
}

// --------------------------------------------------------------------------------
// Name: Get_Validate_Residents
// Abstract: Gets and Validates Residents
// --------------------------------------------------------------------------------
void Get_Validate_Residents(udtSurvey* udtSurvey)
{
	int intValid = 0;
	int intResidents = 0;
	while (intValid == 0)
	{
		printf("Enter Number of Residents in Household: ");
		scanf("%d", &intResidents);
		if (intResidents > 0 && intResidents < 100)
		{
			if (intResidents > 0)
			{
				intValid = 1;
				udtSurvey->intResidents = intResidents;
			}
			else
			{
				printf("Invalid Entry.\n");
			}
		}
		else
		{
			printf("Invalid entry.\n");
			while (getchar() != '\n');
		}
	}
}

// --------------------------------------------------------------------------------
// Name: Get_Validate_Income
// Abstract: Gets and Validates Income
// --------------------------------------------------------------------------------
void Get_Validate_Income(udtSurvey* udtSurvey)
{
	int intValid = 0;
	int intIncome = 0;
	while (intValid == 0)
	{
		printf("Enter Yearly Income for the household: $");
		scanf("%d", &intIncome);
		if (intIncome > 0 && intIncome < 999999999)
		{
			if (intIncome > 0)
			{
				intValid = 1;
				udtSurvey->intIncome = intIncome;
			}
			else
			{
				printf("Invalid Entry.\n");
			}
		}
		else
		{
			printf("Invalid entry.\n");
			while (getchar() != '\n');
		}
	}
}

// --------------------------------------------------------------------------------
// Name: PopulateSurveyArray
// Abstract: Loop to fill struct Array from CSV file
// --------------------------------------------------------------------------------
int PopulateSurveyArray(udtSurvey* udtSurveys, char strFileName[])
{
	char strLine[100] = "\0";
	FILE* pfilInput = NULL;
	int intResult = 0;
	int totalHouseholds = 0;
	int intCounter = 0;

	// Open the file
	intResult = OpenFileRead(strFileName, &pfilInput);
	if (intResult == 1)
	{
		// Skip the header line
		fgets(strLine, sizeof(strLine), pfilInput);

		while (fgets(strLine, sizeof(strLine), pfilInput) != NULL)
		{
			size_t len = strlen(strLine);
			if (len > 0 && strLine[len - 1] == '\n')
			{
				strLine[len - 1] = '\0';
			}

			AddSurveyToArray(strLine, &udtSurveys[intCounter]);
			intCounter += 1;
			ClearString(strLine);
		}
		if (pfilInput != NULL)
		{
			fclose(pfilInput);
		}
		return intCounter;
	}
	else
	{
		printf("Error: File could not be opened.\n");
		return 0;
	}
}

// --------------------------------------------------------------------------------
// Name: AddSurveyToArray
// Abstract: Adds an address to the array
// --------------------------------------------------------------------------------

void AddSurveyToArray(char strSurvey[], udtSurvey* pudtSurvey)
{
	char strNextField[30] = "";
	int intIndex = 0;

	// Append a comma to parse the last field correctly
	int intLength = Length(strSurvey);
	strSurvey[intLength] = ',';

	// Date Recorded
	intIndex = strCopyTilComma(pudtSurvey->strDateRecorded, strSurvey, intIndex);
	ClearString(strNextField);

	// State
	intIndex = strCopyTilComma(pudtSurvey->strState, strSurvey, intIndex);
	ClearString(strNextField);

	// County
	intIndex = strCopyTilComma(pudtSurvey->strCounty, strSurvey, intIndex);
	ClearString(strNextField);


	// Race
	intIndex = strCopyTilComma(pudtSurvey->strRace, strSurvey, intIndex);
	ClearString(strNextField);

	// Residents
	char strInts[20] = "";
	intIndex = strCopyTilComma(strInts, strSurvey, intIndex);
	pudtSurvey->intResidents = atoi(strInts);
	ClearString(strInts);

	// Income
	intIndex = strCopyTilComma(strInts, strSurvey, intIndex);
	pudtSurvey->intIncome = atoi(strInts);
}

//----------------------------------------
// Name: OpenFileRead
// Abstract: Opens the file for reading. Returns 1 if successful, 0 if failed
//----------------------------------------

int OpenFileRead(char strFileName[], FILE** ppfilInput)
{
	*ppfilInput = fopen(strFileName, "r");

	if (*ppfilInput != NULL)
	{
		return 1;
	}
	else
	{
		printf("Error opening %s.\n", strFileName);
		return 0;
	}
}

//----------------------------------------
// Name: OpenFileAppend
// Abstract: Opens the file for reading. Returns 1 if successful, 0 if failed
//----------------------------------------

int OpenFileAppend(char strFileName[], FILE** ppfilInput)
{
	*ppfilInput = fopen(strFileName, "a");

	if (*ppfilInput != NULL)
	{
		return 1;
	}
	else
	{
		printf("Error opening %s.\n", strFileName);
		return 0;
	}
}


//----------------------------------------
// Name: PrintToFile
// Abstract: Prints Structs to file
//----------------------------------------
void PrintToFile(FILE** ppfilOutput, udtSurvey* pudtHead)
{
	udtSurvey* currentSurvey = pudtHead;
	if (currentSurvey == NULL)
	{
		printf("No surveys to print.\n");
		return;
	}

	// Print CSV header (already exists)
	//fprintf(*ppfilOutput, "Date,State,County,Race,Residents,Income\n");

	fprintf(*ppfilOutput, "%s,%s,%s,%s,%d,%d\n",
		currentSurvey->strDateRecorded,
		currentSurvey->strState,
		currentSurvey->strCounty,
		currentSurvey->strRace,
		currentSurvey->intResidents,
		currentSurvey->intIncome);

	fclose(*ppfilOutput);
}

//----------------------------------------
// Name: TotalHouseholdsSurveyed
// Abstract: Will show the total of the overall surveyed.
//----------------------------------------
void TotalHouseholdsSurveyed(char strFileName[])
{
	char strLine[100] = "\0";
	FILE* pfilInput = NULL;
	int intResult = 0;
	int totalHouseholds = 0;

	intResult = OpenFileRead(strFileName, &pfilInput);
	if (intResult == 1)
	{
		// Skip the header line
		fgets(strLine, sizeof(strLine), pfilInput);

		while (fgets(strLine, sizeof(strLine), pfilInput) != NULL)
		{
			totalHouseholds += 1;
		}

		printf("Total Households Surveyed: %d\n", totalHouseholds);

		fclose(pfilInput);
	}
	else
	{
		printf("Error opening the file: %s\n", strFileName);
	}
}

//----------------------------------------
// Name: TotalHouseholdsSurveyedPerCounty
// Abstract: Will show the total of the overall surveyed by county and state.
//----------------------------------------
void TotalHouseholdsSurveyedPerCounty(udtSurvey* udtSurveys, int intLines)
{

	//INDEX: 0 = Ohio, 1 = Kentucky
	int intStates[2] = { 0 };
	//INDEX: 0 = Hamiton, 1 = Butler, 2 = Boone, 3 = Kenton
	int intCounties[4] = { 0 };
	for (int intCounter = 0; intCounter <= intLines; intCounter += 1)
	{
		if (strcmp(udtSurveys[intCounter].strCounty, "Hamilton") == 0)
		{
			intStates[0] += 1;
			intCounties[0] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strCounty, "Butler") == 0)
		{
			intStates[0] += 1;
			intCounties[1] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strCounty, "Boone") == 0)
		{
			intStates[1] += 1;
			intCounties[2] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strCounty, "Kenton") == 0)
		{
			intStates[1] += 1;
			intCounties[3] += 1;
		}
	}

	printf("Total Households by County\n----------------\n");
	printf("Ohio: %d\n", intStates[0]);
	printf("\tHamilton: %d\n", intCounties[0]);
	printf("\tButler: %d\n", intCounties[1]);
	printf("Kentucky: %d\n", intStates[1]);
	printf("\tBoone: %d\n", intCounties[2]);
	printf("\tKenton: %d\n", intCounties[3]);

}

//----------------------------------------
// Name: TotalHouseholdsSurveyedPerRace
// Abstract: Will show the total of the overall surveyed by reported race.
//----------------------------------------
void TotalHouseholdsSurveyedPerRace(udtSurvey* udtSurveys, int intLines)
{
	//INDEX: 0 = Caucasian, 1 = African American, 2 = Hispanic, 3 = Asian, 4 = Other
	int intRaces[5] = { 0 };
	for (int intCounter = 0; intCounter < intLines; intCounter += 1)
	{
		if (strcmp(udtSurveys[intCounter].strRace, "Caucasian") == 0)
		{
			intRaces[0] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strRace, "African American") == 0)
		{
			intRaces[1] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strRace, "Hispanic") == 0)
		{
			intRaces[2] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strRace, "Asian") == 0)
		{
			intRaces[3] += 1;
		}
		else
		{
			intRaces[4] += 1;
		}
	}

	printf("Total Households by Race\n----------------\n");
	printf("Caucasian: %d\n", intRaces[0]);
	printf("African American: %d\n", intRaces[1]);
	printf("Hispanic: %d\n", intRaces[2]);
	printf("Asian: %d\n", intRaces[3]);
	printf("Other: %d\n", intRaces[4]);

}

//----------------------------------------
// Name: AverageHouseholdIncome
// Abstract: Show the overall average household income for those that are surveyed.
//----------------------------------------
void AverageHouseholdIncome(udtSurvey* udtSurveys, int intLines)
{
	int intTotal = 0;
	for (int intCounter = 0; intCounter < intLines; intCounter += 1)
	{
		intTotal += udtSurveys[intCounter].intIncome;
	}
	printf("Average Income for all Households is: %d", (intTotal / intLines));

}

//----------------------------------------
// Name: AverageHouseholdIncomeByCountyAndState
// Abstract: Will display the average household income by state and then by county.
//----------------------------------------
void AverageHouseholdIncomeByCountyAndState(udtSurvey* udtSurveys, int intLines)
{
	//INDEX: 0 = Ohio, 1 = Kentucky, 2 = Hamilton, 3 = Butler, 4 = Boone, 5 = Kenton
	int intTotals[6] = { 0 };
	int intCounts[6] = { 0 };
	for (int intCounter = 0; intCounter < intLines; intCounter += 1)
	{
		if (strcmp(udtSurveys[intCounter].strCounty, "Hamilton") == 0)
		{
			intTotals[0] += udtSurveys[intCounter].intIncome;
			intCounts[0] += 1;
			intTotals[2] += udtSurveys[intCounter].intIncome;
			intCounts[2] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strCounty, "Butler") == 0)
		{
			intTotals[0] += udtSurveys[intCounter].intIncome;
			intCounts[0] += 1;
			intTotals[3] += udtSurveys[intCounter].intIncome;
			intCounts[3] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strCounty, "Boone") == 0)
		{
			intTotals[1] += udtSurveys[intCounter].intIncome;
			intCounts[1] += 1;
			intTotals[4] += udtSurveys[intCounter].intIncome;
			intCounts[4] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strCounty, "Kenton") == 0)
		{
			intTotals[1] += udtSurveys[intCounter].intIncome;
			intCounts[1] += 1;
			intTotals[5] += udtSurveys[intCounter].intIncome;
			intCounts[5] += 1;
		}

	}

	for (int i = 0; i < 6; i += 1)
	{
		if (intCounts[i] != 0)
		{
			intTotals[i] = intTotals[i] / intCounts[i];
		}
	}

	printf("Average Income by County\n----------------\n");
	printf("Ohio: %d\n", intTotals[0]);
	printf("\tHamilton: %d\n", intTotals[2]);
	printf("\tButler: %d\n", intTotals[3]);
	printf("Kentucky: %d\n", intTotals[1]);
	printf("\tBoone: %d\n", intTotals[4]);
	printf("\tKenton: %d\n", intTotals[5]);

}

//----------------------------------------
// Name: AverageHouseholdIncomeByRace
// Abstract: Will display the average household income by race.
//----------------------------------------
void AverageHouseholdIncomeByRace(udtSurvey* udtSurveys, int intLines)
{
	//INDEX: 0 = Caucasian, 1 = African American, 2 = Hispanic, 3 = Asian, 4 = Other
	int intRaces[5] = { 0 };
	int intCounts[5] = { 0 };
	for (int intCounter = 0; intCounter < intLines; intCounter += 1)
	{
		if (strcmp(udtSurveys[intCounter].strRace, "Caucasian") == 0)
		{
			intRaces[0] += udtSurveys[intCounter].intIncome;
			intCounts[0] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strRace, "African American") == 0)
		{
			intRaces[1] += udtSurveys[intCounter].intIncome;
			intCounts[1] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strRace, "Hispanic") == 0)
		{
			intRaces[2] += udtSurveys[intCounter].intIncome;
			intCounts[2] += 1;
		}
		else if (strcmp(udtSurveys[intCounter].strRace, "Asian") == 0)
		{
			intRaces[3] += udtSurveys[intCounter].intIncome;
			intCounts[3] += 1;
		}
		else
		{
			intRaces[4] += udtSurveys[intCounter].intIncome;
			intCounts[4] += 1;
		}
	}

	for (int i = 0; i < 5; i += 1)
	{
		if (intCounts[i] != 0)
		{
			intRaces[i] = intRaces[i] / intCounts[i];
		}
		
	}

	printf("Average Income by Race\n----------------\n");
	printf("Caucasian: %d\n", intRaces[0]);
	printf("African American: %d\n", intRaces[1]);
	printf("Hispanic: %d\n", intRaces[2]);
	printf("Asian: %d\n", intRaces[3]);
	printf("Other: %d\n", intRaces[4]);

}

//----------------------------------------
// Name: PercentageBelowPoverty
// Abstract: Show the overall percentage of those below poverty based on the table below for all data collected.
//----------------------------------------
void PercentageBelowPoverty(udtSurvey* udtSurveys, int intLines)
{
	//INDEX: 0 = 1 Resident, 1 = 2 Residents, 2 = 3 Residents, 3 = 4 Residents, 4 = 5+ Residents
	int intTotals[5] = { 0 };

	int intCounter = 0;
	for (int intCounter = 0; intCounter < intLines; intCounter += 1)
	{
		switch (udtSurveys[intCounter].intResidents)
		{
		case 1:
			if (udtSurveys[intCounter].intIncome < 12000)
			{
				intTotals[0] += 1;
			}
			break;
		case 2:
			if (udtSurveys[intCounter].intIncome < 18000)
			{
				intTotals[1] += 1;
			}
			break;
		case 3:
			if (udtSurveys[intCounter].intIncome < 25000)
			{
				intTotals[2] += 1;
			}
			break;
		case 4:
			if (udtSurveys[intCounter].intIncome < 30000)
			{
				intTotals[3] += 1;
			}
			break;
		default:
			if (udtSurveys[intCounter].intIncome < 40000)
			{
				intTotals[4] += 1;
			}
			break;
		}
	}

	printf("Households in Poverty\n-------------------\n");
	printf("1 Resident: %d\n", intTotals[0]);
	printf("2 Residents: %d\n", intTotals[1]);
	printf("3 Residents: %d\n", intTotals[2]);
	printf("4 Residents: %d\n", intTotals[3]);
	printf("5+ Residents: %d\n", intTotals[4]);
}

//----------------------------------------
// Name: PercentageBelowPovertyByCountyAndState
// Abstract: Show the overall percentage of those below poverty based on the table below for all data collected by county and state.
//----------------------------------------
void PercentageBelowPovertyByCountyAndState(udtSurvey* udtSurveys, int intLines)
{
	//INDEX: 0 = Ohio, 1 = Kentucky, 2 = Hamilton, 3 = Butler, 4 = Boone, 5 = Kenton
	int intTotals[6] = { 0 };

	int intCounter = 0;
	for (int intCounter = 0; intCounter < intLines; intCounter += 1)
	{
		switch (udtSurveys[intCounter].intResidents)
		{
		case 1:
			if (udtSurveys[intCounter].intIncome < 12000)
			{
				switch (SortByCounty(udtSurveys[intCounter]))
				{
				case 1:
					intTotals[0] += 1;
					intTotals[2] += 1;
					break;
				case 2:
					intTotals[0] += 1;
					intTotals[3] += 1;
					break;
				case 3:
					intTotals[1] += 1;
					intTotals[4] += 1;
					break;
				case 4:
					intTotals[1] += 1;
					intTotals[5] += 1;
					break;
				}
			}
			break;
		case 2:
			if (udtSurveys[intCounter].intIncome < 18000)
			{
				switch (SortByCounty(udtSurveys[intCounter]))
				{
				case 1:
					intTotals[0] += 1;
					intTotals[2] += 1;
					break;
				case 2:
					intTotals[0] += 1;
					intTotals[3] += 1;
					break;
				case 3:
					intTotals[1] += 1;
					intTotals[4] += 1;
					break;
				case 4:
					intTotals[1] += 1;
					intTotals[5] += 1;
					break;
				}
			}
			break;
		case 3:
			if (udtSurveys[intCounter].intIncome < 25000)
			{
				switch (SortByCounty(udtSurveys[intCounter]))
				{
				case 1:
					intTotals[0] += 1;
					intTotals[2] += 1;
					break;
				case 2:
					intTotals[0] += 1;
					intTotals[3] += 1;
					break;
				case 3:
					intTotals[1] += 1;
					intTotals[4] += 1;
					break;
				case 4:
					intTotals[1] += 1;
					intTotals[5] += 1;
					break;
				}
			}
			break;
		case 4:
			if (udtSurveys[intCounter].intIncome < 30000)
			{
				switch (SortByCounty(udtSurveys[intCounter]))
				{
				case 1:
					intTotals[0] += 1;
					intTotals[2] += 1;
					break;
				case 2:
					intTotals[0] += 1;
					intTotals[3] += 1;
					break;
				case 3:
					intTotals[1] += 1;
					intTotals[4] += 1;
					break;
				case 4:
					intTotals[1] += 1;
					intTotals[5] += 1;
					break;
				}
			}
			break;
		default:
			if (udtSurveys[intCounter].intIncome < 40000)
			{
				switch (SortByCounty(udtSurveys[intCounter]))
				{
				case 1:
					intTotals[0] += 1;
					intTotals[2] += 1;
					break;
				case 2:
					intTotals[0] += 1;
					intTotals[3] += 1;
					break;
				case 3:
					intTotals[1] += 1;
					intTotals[4] += 1;
					break;
				case 4:
					intTotals[1] += 1;
					intTotals[5] += 1;
					break;
				}
			}
			break;
		}
	}

	printf("Households in Poverty by County and State\n-------------------\n");
	printf("Ohio: %d\n", intTotals[0]);
	printf("\tHamilton: %d\n", intTotals[2]);
	printf("\tButler: %d\n", intTotals[3]);
	printf("Ohio: %d\n", intTotals[1]);
	printf("\tHamilton: %d\n", intTotals[4]);
	printf("\tButler: %d\n", intTotals[5]);
}

//----------------------------------------
// Name: SortByCounty
// Abstract: Returns an int specified to a county
//----------------------------------------
int SortByCounty(udtSurvey udtSurvey)
{

	if (strcmp(udtSurvey.strCounty, "Hamilton"))
	{
		return 1;
	}
	else if (strcmp(udtSurvey.strCounty, "Butler"))
	{
		return 2;
	}
	else if (strcmp(udtSurvey.strCounty, "Boone"))
	{
		return 3;
	}
	else if (strcmp(udtSurvey.strCounty, "Kenton"))
	{
		return 4;
	}
}


//----------------------------------------
// Name: PercentageBelowPovertyByRace
// Abstract: Show the overall percentage of those below poverty based on the table below for all data collected by race.
//----------------------------------------
void PercentageBelowPovertyByRace(udtSurvey* udtSurveys, int intLines)
{
	//INDEX: 0 = Caucasian, 1 = African American, 2 = Hispanic, 3 = Asian, 4 = Other
	int intTotals[5] = { 0 };

	int intCounter = 0;
	for (int intCounter = 0; intCounter < intLines; intCounter += 1)
	{
		switch (udtSurveys[intCounter].intResidents)
		{
		case 1:
			if (udtSurveys[intCounter].intIncome < 12000)
			{
				switch (SortByRace(udtSurveys[intCounter]))
				{
				case 1:
					intTotals[0] += 1;
					break;
				case 2:
					intTotals[1] += 1;
					break;
				case 3:
					intTotals[2] += 1;
					break;
				case 4:
					intTotals[3] += 1;
					break;
				case 5:
					intTotals[4] += 1;
					break;
				}
			}
			break;
		case 2:
			if (udtSurveys[intCounter].intIncome < 18000)
			{
				switch (SortByRace(udtSurveys[intCounter]))
				{
				case 1:
					intTotals[0] += 1;
					break;
				case 2:
					intTotals[1] += 1;
					break;
				case 3:
					intTotals[2] += 1;
					break;
				case 4:
					intTotals[3] += 1;
					break;
				case 5:
					intTotals[4] += 1;
					break;
				}
			}
			break;
		case 3:
			if (udtSurveys[intCounter].intIncome < 25000)
			{
				switch (SortByRace(udtSurveys[intCounter]))
				{
				case 1:
					intTotals[0] += 1;
					break;
				case 2:
					intTotals[1] += 1;
					break;
				case 3:
					intTotals[2] += 1;
					break;
				case 4:
					intTotals[3] += 1;
					break;
				case 5:
					intTotals[4] += 1;
					break;
				}
			}
			break;
		case 4:
			if (udtSurveys[intCounter].intIncome < 30000)
			{
				switch (SortByRace(udtSurveys[intCounter]))
				{
				case 1:
					intTotals[0] += 1;
					break;
				case 2:
					intTotals[1] += 1;
					break;
				case 3:
					intTotals[2] += 1;
					break;
				case 4:
					intTotals[3] += 1;
					break;
				case 5:
					intTotals[4] += 1;
					break;
				}
			}
			break;
		default:
			if (udtSurveys[intCounter].intIncome < 40000)
			{
				switch (SortByRace(udtSurveys[intCounter]))
				{
				case 1:
					intTotals[0] += 1;
					break;
				case 2:
					intTotals[1] += 1;
					break;
				case 3:
					intTotals[2] += 1;
					break;
				case 4:
					intTotals[3] += 1;
					break;
				case 5:
					intTotals[4] += 1;
					break;
				}
			}
			break;
		}
	}

	printf("Households in Poverty by Race\n-------------------\n");
	printf("Caucasian: %d\n", intTotals[0]);
	printf("African American: %d\n", intTotals[1]);
	printf("Hispanic: %d\n", intTotals[2]);
	printf("Asian: %d\n", intTotals[3]);
	printf("Other: %d\n", intTotals[4]);
}

//----------------------------------------
// Name: SortByRace
// Abstract: Returns an int specified to a race
//----------------------------------------
int SortByRace(udtSurvey udtSurvey)
{
	if (strcmp(udtSurvey.strRace, "Caucasian") == 0)
	{
		return 1;
	}
	else if (strcmp(udtSurvey.strRace, "African American") == 0)
	{
		return 2;
	}
	else if (strcmp(udtSurvey.strRace, "Hispanic") == 0)
	{
		return 3;
	}
	else if (strcmp(udtSurvey.strRace, "Asian") == 0)
	{
		return 4;
	}
	else
	{
		return 5;
	}
}

int GetEntries(char strFileName[])
{
	FILE* pfilInput = NULL;
	int intResult = 0;
	intResult = OpenFileRead(strFileName, &pfilInput);
	char strLine[100];
	int intCount = 0;
	while (fgets(strLine, sizeof(strLine), pfilInput) != NULL)
	{
		intCount += 1;
	}
	fclose(pfilInput);
	return intCount;
}

//----------------------------------------
// Name: StrCopy
// Abstract: Copies strSource to strDestination
//----------------------------------------
void strCopy(char strDestination[], char strSource[])
{
	int intIndex = 0;

	while (strSource[intIndex] != '\0') {
		strDestination[intIndex] = strSource[intIndex];
		intIndex += 1;
	}
	strDestination[intIndex] = '\0';
}

//----------------------------------------
// Name: StrCopyTilComma
// Abstract: Function for AddSurveyToArray like strCopy but for commas 
//----------------------------------------
int strCopyTilComma(char strDestination[], char strSource[], int intIndex)
{
	int intCounter = 0;
	while (strSource[intIndex] != ',') {
		strDestination[intCounter] = strSource[intIndex];
		intIndex += 1;
		intCounter += 1;
	}
	strDestination[intCounter] = '\0';
	return intIndex + 1;
}

//----------------------------------------
// Name: ClearString
// Abstract: Empties string
//----------------------------------------

void ClearString(char strDestination[])
{
	int intCounter = 0;
	while (strDestination[intCounter] != '\0')
	{
		strDestination[intCounter] = '\0';
		intCounter += 1;
	}
}

//----------------------------------------
// Name: Length
// Abstract: Returns the length of the content in strSource
//----------------------------------------

int Length(char strSource[])
{
	int intCounter = 0;
	while (strSource[intCounter] != '\0')
	{
		intCounter += 1;
	}
	return intCounter;
}

// --------------------------------------------------------------------------------
// Name: PrintOptions
// Abstract: Prints options so main function is less jumbled
// --------------------------------------------------------------------------------
void PrintOptions()
{
	printf("\n\nSelect from Below\n----------------------\n");
	printf("'A' - Total Households Surveyed\n");
	printf("'B' - Total Households Surveyed Per County\n");
	printf("'C' - Total Households Surveyed Per Race\n");
	printf("'D' - Average Household Income\n");
	printf("'E' - Average Household Income by County and State\n");
	printf("'F' - Average Household Income by Race\n");
	printf("'G' - Percentage Below Poverty\n");
	printf("'H' - Percentage Below Poverty by County and State\n");
	printf("'I' - Percentage Below Poverty by Race\n");
}

