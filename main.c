#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Cheatcode: wwddsasdssawasaawdwawdwa

enum feld_type{
  empty = 1, brunnen, relikt, danger
};

typedef struct feld {
    char feldDar; //if player = 1 --> '0' else '?'
    int nummer; //1-25
    int player; //0 or 1
    enum feld_type type;
}feld;

typedef struct player {
    int lebenspunkte;
    int relikte;
    int rauchbombe;
    int knieschoner;
    int trank;
    int strenght;
    int defence;
    int knowledge;
    int xkoordinate;
    int ykoordinate;
}player;

void printFeld(feld* spielfeld[5][5]){
    for (int i = 0; i != 5; ++i){
      for (int j = 0; j != 5; ++j){
        printf("%c ", (spielfeld[i][j])->feldDar);
      }
      printf("\n");
    }
}

void updatePlayer(feld* spielfeld[5][5]){
    for (int i = 0; i != 5; ++i){
      for (int j = 0; j != 5; ++j){
        if(spielfeld[i][j]->player == 1){
            spielfeld[i][j]->feldDar = '0';
            break;
        }
      }
    }
}

void generatingMap(feld* spielfeld[5][5]){
    (srand(time(0)));
    int nummer = 1;
    int y = 2;
    //Automatisches befüllen des Arrays
    for(int i = 0; i < 5; i++) {
            int x = -2;
      for(int j = 0; j < 5; j++) {
        //create random Number from 1-10
        int probability = (rand()% 9) + 1;
        spielfeld[i][j] = malloc(sizeof(feld));
        //Player bekommt darstellung '0'
        spielfeld[i][j]->feldDar = '?';
        spielfeld[i][j]->nummer = nummer;
        //Generate field type
        if(probability <= 4){ //Leeres Feld
            spielfeld[i][j]->type = 1;
        }
        else if(probability == 5){ //Regeneration
            spielfeld[i][j]->type = 2;
        }
        else if(probability == 6){ //Relikt
            spielfeld[i][j]->type = 3;
        }
        else if(probability >= 7){ //Gefahrenfeld
            spielfeld[i][j]->type = 4;
        }
        nummer++;
        x++;
      }
      y--;
   }
   spielfeld[2][2]->feldDar = '0';
   spielfeld[2][2]->type = 1; //Relikt kann nicht auf spawn sein (Verhindert sonst Ende des Spiels)
}

void checkRelikt(feld* spielfeld[5][5]){
    int checkRelikte = 0;
    //Checken ob Relikte am Feld sind
    for (int i = 0; i != 5; ++i){
      for (int j = 0; j != 5; ++j){
        if(spielfeld[i][j]->type == 3){
            checkRelikte++;
        }
      }
    }
    //2 Zahlen (eine von 1-12 eine von 14-25 (auf 13 ist der Player)) generieren und Relikte platzieren
    if(checkRelikte == 0){
        int fieldNumberFirst = (rand()% 12);
        int fieldNumberSecond = (rand()% 25) + 13;
        for (int x = 0; x != 5; ++x){
              for (int y = 0; y != 5; ++y){
                if(spielfeld[x][y]->nummer == fieldNumberFirst || spielfeld[x][y]->nummer == fieldNumberSecond){
                    spielfeld[x][y]->type = 3;
                }
            }
        }
    }
}

void takeAction(feld* spielfeld[5][5], player* player, int *heilung, int *schaden, int relikteImSpiel, int *relikteGefunden){
    (srand(time(0)));
    //Empty field
    if(spielfeld[player->xkoordinate][player->ykoordinate]->type == 1){
        printf("\n\nWeit und breit nichts zu sehen. Wohin geht es jetzt?\n");
    }
    //Brunnen
    else if(spielfeld[player->xkoordinate][player->ykoordinate]->type == 2){
        printf("\nDu findest einen Brunnen und regenerierst einen Lebenspunkt.");
        player->lebenspunkte++;
        *heilung = *heilung + 1;
        printf("\nDu hast nun %d Lebenspunkte", player->lebenspunkte);
        //50% chance of running this
        int findItem = (rand()% 9) + 1; //number 1-10
        if(findItem > 3){ //70% chance to find item
            int typeOfItem = (rand()% 3) + 1; //number from 1-3
            if(typeOfItem == 1){
                player->rauchbombe++;
                printf("\nDu hast Gl""\x81""ck und findest eine Rauchbombe!");
            }
            else if(typeOfItem == 2){
                player->knieschoner++;
                printf("\nDu hast Gl""\x81""ck und findest Knieschoner!");
            }
            else if(typeOfItem == 3){
                player->trank++;
                printf("\nDu hast Gl""\x81""ck und findest einen Trank des Durchblickens!");
            }
        }
    }
    //Relikt
    else if(spielfeld[player->xkoordinate][player->ykoordinate]->type == 3){
        printf("\nGut gemacht! Du findest ein Relikt!");
        player->relikte++;
        *relikteGefunden = *relikteGefunden + 1;
        printf("\nDu hast %d von %d Relikte gesammelt", player->relikte, relikteImSpiel);
        //50% chance of running this
        int findItem = (rand()% 9) + 1; //number 1-10
        if(findItem > 5){ //50% chance to find item
            int typeOfItem = (rand()% 3) + 1; //number 1-3
            if(typeOfItem == 1){
                player->rauchbombe++;
                printf("\nDu hast Gl""\x81""ck und findest eine Rauchbombe!");
            }
            else if(typeOfItem == 2){
                player->knieschoner++;
                printf("\nDu hast Gl""\x81""ck und findest Knieschoner!");
            }
            else if(typeOfItem == 3){
                player->trank++;
                printf("\nDu hast Gl""\x81""ck und findest einen Trank des Durchblickens!");
            }
        }
    }
    //Danger
    else if(spielfeld[player->xkoordinate][player->ykoordinate]->type == 4){
        int typeOfDanger = (rand()% 3) + 1;
        //Chance of escaping attack
        int escape = (rand()% 2) + 1;

        if(typeOfDanger == 1){
            printf("\nACHTUNG! 2 Gestalten wollen dich ausrauben. Kannst du dich verteidigen?");
            if(player->rauchbombe > 0){
                player->rauchbombe--;
                printf("\nDu wwirfst eine Rauchbombe und entkommst den Angreifern!");
            }
            else{
                if(player->strenght <= 2){ //if strenght modifier is bigger than 2 you can defend yourself
                    if(escape == 1){
                        player->lebenspunkte--;
                        *schaden = *schaden + 1;
                        printf("\nDu wurdest von den Gestalten ""\x81""berw\x84ltigt und verlierst einen Lebenspunkt!");
                        printf("\nDu hast momentan noch %d Lebenspunkte", player->lebenspunkte);
                    }
                    else{
                        printf("\nDu konntest die Angreifer besiegen und kannst nun weiterziehen!");
                    }
                }
                else{
                    printf("\nDu konntest die Angreifer besiegen und kannst nun weiterziehen!");
                }
            }
        }
        else if(typeOfDanger == 2){
            printf("\nACHTUNG! Du bist unachtsam und f\x84llst hin! Wirst du dich verletzen?");
            if(player->knieschoner > 0){
                player->knieschoner--;
                printf("\nDu hast Kniesch\x81tzer und verletzt dich nicht! Dabei gehen sie aber leider kaputt.");
            }
            else{
                if(player->defence <= 2){
                    if(escape == 1){
                        player->lebenspunkte--;
                        *schaden = *schaden + 1;
                        printf("\nDu hast dir dein Knie verletzt!");
                        printf("\nDu hast momentan noch %d Lebenspunkte", player->lebenspunkte);
                    }
                    else{
                        printf("\nDu konntest dich abrollen und kannst nun weiterziehen!");
                    }
                }
                else{
                    printf("\nDu konntest dich abrollen und kannst nun weiterziehen!");
                }
            }
        }
        else if(typeOfDanger == 3){
            printf("\nACHTUNG! Ein H\x81tchenspieler will mit dir spielen. Wird er dich ""\x81""berlisten?");
            if(player->trank > 0){
                player->trank--;
                printf("\nDu trinkst einen Trank der Durchsicht und durchschaust den H\x81tchenspieler.");
            }
            else{
                if(player->knowledge <= 2){
                    if(escape == 1){
                        player->lebenspunkte--;
                        *schaden = *schaden + 1;
                        printf("\nDu hast deinen Einsatz verloren und leidest unter dem entstandenen psychischen Schaden!");
                        printf("\nDu hast momentan noch %d Lebenspunkte", player->lebenspunkte);
                    }
                    else{
                        printf("\nDu konntest den H\x81tchenspieler durchschauen und kannst nun weiterziehen!");
                    }
                }
                else{
                    printf("\nDu konntest den H\x81tchenspieler durchschauen und kannst nun weiterziehen!");
                }
            }
        }
    }
}

int enemyAttack(feld* spielfeld[5][5], player* player, int *schaden, int *interaktion, int *enemyCount){
    for(int i = 1; i < *enemyCount; i++){ //number of enemys bzw how often enemy moves
        int enemyX = (rand()% 4);
        int enemyY = (rand()% 4);
        if(spielfeld[player->xkoordinate][player->ykoordinate] == spielfeld[enemyX][enemyY]){
            player->lebenspunkte--;
            *schaden = *schaden + 1;
            //printf("\nACHTUNG! Gegner %d hat dich entdeckt und du verlierst ein Leben. Du hast momentan noch %d Lebenspunkte", i, player->lebenspunkte);
            *interaktion = *interaktion + 1;
            return 1;
        }
    }
    printf("\n");
    return 0;
}

void freeAll(feld* spielfeld[5][5]){
    for (int i = 0; i != 5; ++i){
      for (int j = 0; j != 5; ++j){
        free(spielfeld[i][j]);
      }
    }
}

void testPrint(feld* spielfeld[5][5]){
    for (int i = 0; i != 5; ++i){
      for (int j = 0; j != 5; ++j){
        printf("Spielfeld[%d][%d] %d: ", i, j, spielfeld[i][j]->nummer);
        printf("Player: %d ", spielfeld[i][j]->player);
        printf("Darstellung: %c ", spielfeld[i][j]->feldDar);
      }
      printf("\n");
    }
}

int countRelikte(feld* spielfeld[5][5]){
    int relikteImSpiel = 0;
    for (int i = 0; i != 5; ++i){
      for (int j = 0; j != 5; ++j){
            if(spielfeld[i][j]->type == 3){
                relikteImSpiel++;
            }
        }
    }
    return relikteImSpiel;
}

void createMap(feld* spielfeld[5][5]){
    generatingMap(spielfeld);//generiert map
    checkRelikt(spielfeld); //check ob Relikt vorhanden ist
}

void printStats(player* player, int *level, int *relikteImSpiel, int *enemyCount){
    printf("\nLevel %d", *level);
    printf("\nLeben: %d", player->lebenspunkte);
    printf("\nStrenght, Defence, Knowledge (%d/%d/%d)", player->strenght, player->defence, player->knowledge);
    printf("\nRauchbombe, Knieschoner, Trank (%d/%d/%d)", player->rauchbombe, player->knieschoner, player->trank);
    printf("\nRelikte: %d/%d", player->relikte, *relikteImSpiel);
    printf("\nGegner: %d\n\n", *enemyCount);
}

void emptyField(feld* spielfeld[5][5], player* player){
    spielfeld[player->xkoordinate][player->ykoordinate]->type = 1;
    spielfeld[player->xkoordinate][player->ykoordinate]->player = 0;
    spielfeld[player->xkoordinate][player->ykoordinate]->feldDar = '-';
}

void levelUp(player* player){
    char input;
    while(1){
        printf("\nWählen Sie welches Attribut Sie verbessern wollen strenght[s], defence[d] oder knowdledge[k]: ");
        scanf(" %c", &input);
        if(input == 's'){
            player->strenght++;
            break;
        }
        else if(input == 'd'){
            player->defence++;
            break;
        }
        else if(input == 'k'){
            player->knowledge++;
            break;
        }
        else{
            printf("\nDas war keine g\x81ltige Eingabe! Versuchen Sie es bitte nochmal.");
        }
    }
}

void gameplay(feld* spielfeld[5][5], player* player, int *schaden, int *interaktion, int *enemyCount, int *heilung, int *relikteGefunden, int *level){
    char input;
    player->relikte = 0;
    printf("\nLevel %d", *level);
    printf("\nLeben: %d", player->lebenspunkte);
    printf("\nStrenght, Defence, Knowledge (%d/%d/%d)", player->strenght, player->defence, player->knowledge);
    printf("\nRauchbombe, Knieschoner, Trank (%d/%d/%d)", player->rauchbombe, player->knieschoner, player->trank);

    createMap(spielfeld);

    int relikteImSpiel = countRelikte(spielfeld);
    printf("\nRelikte: %d/%d", player->relikte, relikteImSpiel);
    printf("\nGegner: %d\n\n", *enemyCount);

    printFeld(spielfeld); //Gibt das Feld grafisch aus

    player->xkoordinate = 2;
    player->ykoordinate = 2;

    levelUp(player);

    while(1){
        printf("\nBewegen Sie den Charakter in dem Sie 'w', 'a', 's' oder 'd' eingeben: ");
        scanf(" %c", &input);
        int enemyMessage = 0;
        //printf("Du hast gerade %d Relikte gesammelt", relikte);
        if(input == 'w'){
            system("cls");
            if(player->xkoordinate == 0){
                printf("\n\nHier geht es nicht weiter! Versuchen Sie es bitte nochmal.\n");
                printStats(player, level, &relikteImSpiel, enemyCount);
                printFeld(spielfeld);
            }
            else{
                emptyField(spielfeld, player);
                player->xkoordinate--;
                spielfeld[player->xkoordinate][player->ykoordinate]->player = 1;
                updatePlayer(spielfeld);
                takeAction(spielfeld, player, heilung, schaden, relikteImSpiel, relikteGefunden);
                if(player->lebenspunkte < 1){
                    printf("\nDu hast alle deine Lebenspunkte und somit das Spiel verloren. Probiere es doch nochmal!\n");
                    printFeld(spielfeld);
                    break;
                }
                //enemyAttack(spielfeld, player, schaden, interaktion, enemyCount);
                if(enemyAttack(spielfeld, player, schaden, interaktion, enemyCount)){
                    enemyMessage = 1;
                }
                printStats(player, level, &relikteImSpiel, enemyCount);
                printFeld(spielfeld);
                if(enemyMessage){
                    printf("\nACHTUNG! Ein Gegner hat dich entdeckt und du verlierst ein Leben. Du hast momentan noch %d Lebenspunkte", player->lebenspunkte);
                }
            }
        }
        else if(input == 's'){
            system("cls");
            if(player->xkoordinate == 4){
                printf("\n\nHier geht es nicht weiter! Versuchen Sie es bitte nochmal.\n");
                printStats(player, level, &relikteImSpiel, enemyCount);
                printFeld(spielfeld);
            }
            else{
                emptyField(spielfeld, player);
                player->xkoordinate++;
                spielfeld[player->xkoordinate][player->ykoordinate]->player = 1;
                updatePlayer(spielfeld);
                takeAction(spielfeld, player, heilung, schaden, relikteImSpiel, relikteGefunden);
                if(player->lebenspunkte < 1){
                    printf("\nDu hast alle deine Lebenspunkte und somit das Spiel verloren. Probiere es doch nochmal!");
                    printFeld(spielfeld);
                    break;
                }
                if(enemyAttack(spielfeld, player, schaden, interaktion, enemyCount)){
                    enemyMessage = 1;
                }
                printStats(player, level, &relikteImSpiel, enemyCount);
                printFeld(spielfeld);
                if(enemyMessage){
                    printf("\nACHTUNG! Ein Gegner hat dich entdeckt und du verlierst ein Leben. Du hast momentan noch %d Lebenspunkte", player->lebenspunkte);
                }
            }
        }
        else if(input == 'a'){
            system("cls");
            if(player->ykoordinate == 0){
                printf("\n\nHier geht es nicht weiter! Versuchen Sie es bitte nochmal.\n");
                printStats(player, level, &relikteImSpiel, enemyCount);
                printFeld(spielfeld);
            }
            else{
                emptyField(spielfeld, player);
                player->ykoordinate--;
                spielfeld[player->xkoordinate][player->ykoordinate]->player = 1;
                updatePlayer(spielfeld);
                takeAction(spielfeld, player, heilung, schaden, relikteImSpiel, relikteGefunden);
                if(player->lebenspunkte < 1){
                    printf("\nDu hast alle deine Lebenspunkte und somit das Spiel verloren. Probiere es doch nochmal!");
                    printFeld(spielfeld);
                    break;
                }
                if(enemyAttack(spielfeld, player, schaden, interaktion, enemyCount)){
                    enemyMessage = 1;
                }
                printStats(player, level, &relikteImSpiel, enemyCount);
                printFeld(spielfeld);
                if(enemyMessage){
                    printf("\nACHTUNG! Ein Gegner hat dich entdeckt und du verlierst ein Leben. Du hast momentan noch %d Lebenspunkte", player->lebenspunkte);
                }
            }
        }
        else if(input == 'd'){
            system("cls");
            if(player->ykoordinate == 4){
                printf("\n\nHier geht es nicht weiter! Versuchen Sie es bitte nochmal.\n");
                printStats(player, level, &relikteImSpiel, enemyCount);
                printFeld(spielfeld);
            }
            else{
                emptyField(spielfeld, player);
                player->ykoordinate++;
                spielfeld[player->xkoordinate][player->ykoordinate]->player = 1;
                updatePlayer(spielfeld);
                takeAction(spielfeld, player, heilung, schaden, relikteImSpiel, relikteGefunden);
                if(player->lebenspunkte < 1){
                    printf("\nDu hast alle deine Lebenspunkte und somit das Spiel verloren. Probiere es doch nochmal!");
                    printFeld(spielfeld);
                    break;
                }
                if(enemyAttack(spielfeld, player, schaden, interaktion, enemyCount)){
                    enemyMessage = 1;
                }
                printStats(player, level, &relikteImSpiel, enemyCount);
                printFeld(spielfeld);
                if(enemyMessage){
                    printf("\nACHTUNG! Ein Gegner hat dich entdeckt und du verlierst ein Leben. Du hast momentan noch %d Lebenspunkte", player->lebenspunkte);
                }
            }
        }
        else{
            printf("\nDas war leider keine g\x81ltige Eingabe! Versuchen Sie es bitte nochmal.");
        }
        //printf("\nGerade befindest du dich auf einem %d-Feld", spielfeld[currX][currY]->type);
        if(player->relikte == relikteImSpiel){
            printf("\nDU HAST ALLE RELIKTE GEFUNDEN UND DAS LEVEL GESCHAFFT!");
            break;
        }
        else if(player->lebenspunkte < 1){
            printf("\nDu hast alle deine Lebenspunkte und somit das Spiel verloren. Probiere es doch nochmal!");
            break;
        }
        //testPrint(spielfeld);
    }
}

int main()
{
    (srand(time(0)));
    feld* spielfeld[5][5];
    player* player = malloc(sizeof(*player));
    //player stats
    player->relikte = 0;
    player->lebenspunkte = 5;
    player->rauchbombe = 0;
    player->knieschoner = 0;
    player->trank = 0;
    player->strenght = 0;
    player->defence = 0;
    player->knowledge = 0;
    player->xkoordinate =  2;
    player->ykoordinate =  2;
    //Statistiken
    int heilung = 0;
    int schaden = 0;
    int interaktion = 0;
    int enemyCount = 4;
    int relikteGefunden = 0;
    int level = 1;

    while(1){
        gameplay(spielfeld, player, &schaden, &interaktion, &enemyCount, &heilung, &relikteGefunden, &level);
        enemyCount++;
        level++;
        if(player->lebenspunkte < 1){
            break;
        }
    }
    printf("\nDeine Statistiken:");
    printf("\n%d Relikte wurden gefunden", relikteGefunden);
    printf("\n%d Schaden wurden erlitten", schaden);
    printf("\n%d Leben wurden geheilt", heilung);
    printf("\n%d-Mal wurdest du angegriffen", interaktion);
    printf("\nDanke f\x81rs Spielen!");
    freeAll(spielfeld);
    return 0;
}
