#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int number;
    char activity[101];
    int score;
} Student;

// conta atividades distintas
int distinctActivities(char *s) {
    int seen[256] = {0};
    int count = 0;

    for (int i = 0; s[i]; i++) {
        if (!seen[(unsigned char)s[i]]) {
            seen[(unsigned char)s[i]] = 1;
            count++;
        }
    }
    return count;
}

// maior sequência consecutiva
int longestStreak(char *s) {
    int max = 0, curr = 1;

    for (int i = 1; s[i]; i++) {
        if (s[i] == s[i-1]) {
            curr++;
        } else {
            if (curr > max) max = curr;
            curr = 1;
        }
    }
    if (curr > max) max = curr;

    return max;
}

// calcula score
int engagement(char *activity) {
    return distinctActivities(activity) * longestStreak(activity);
}

// comparação para qsort
int cmp(const void *a, const void *b) {
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;

    if (s1->score != s2->score)
        return s2->score - s1->score; // decrescente

    return s1->number - s2->number;   // crescente
}

int main() {
    Student turma[] = {
        {5, "EEPRRPP"}, {8, "LLLSSLL"}, {24, "RPPRRRL"},
        {3, "SSSPPSS"}, {1, "PPPEEEPP"}, {15, "LRLLLL"},
        {11, "ERRREE"}, {2, "SSSLLSS"}, {7, "PPLLPLPL"},
        {12, "RRSSSRR"}, {19, "EEERRREE"}, {4, "LLPPSSLL"},
        {9, "SSSSEEEESL"}, {16, "PPPRRLLL"}, {21, "LLSSSLLL"},
        {6, "ERRRRPPP"}, {13, "PPPPSSPP"}, {25, "RRRLLERR"},
        {10, "SSPPPPSS"}, {18, "LLLEEEELL"}, {14, "EEERRREEE"},
        {17, "SSSPPPSSS"}, {20, "LLLEEERRR"}, {22, "PPPLLLL"},
        {23, "SSSRRRSSS"}, {26, "EEEPPPLL"}, {27, "LLRRRPPP"}
    };

    int n = sizeof(turma) / sizeof(turma[0]);

    // calcular scores
    for (int i = 0; i < n; i++) {
        turma[i].score = engagement(turma[i].activity);
    }

    // ordenar
    qsort(turma, n, sizeof(Student), cmp);

    // posição 4 (índice 3)
    printf("%d\n", turma[3].number);

    return 0;
}