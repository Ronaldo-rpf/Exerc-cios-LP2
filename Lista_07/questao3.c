#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uchar;

typedef struct{
    uchar R, G, B;
}tRGB;

typedef struct{
    uchar *_img;
    uchar **img;
    int nLin, nCol;
}imgGray;

typedef struct{
    tRGB *_img;
    tRGB **img;
    int nLin, nCol;
}imgRGB;

struct imagens{
    imgGray img;
    struct imagens *prox;
};

typedef struct imagens tImagens;

//---------------------------------------------------------------------------------------------------------------------------------------------

imgGray alocaImagemGray (int nLin, int nCol);

imgRGB alocaImagemRGB (int nLin, int nCol);

int saveImgGray (imgGray img, char desc[], char fileName[]);

int saveImgRGB(imgRGB img, char desc[], char fileName[]);

int loadHead(char fileName[], char desc[]);

int main (){
    int aux;
    imgGray picgray;
    imgRGB picRGB;
    picgray = alocaImagemGray (5, 10);
    if (picgray.img == NULL || picgray._img == NULL){
        printf("Erro ao alocar imagem Gray.\n\n");
        return 1;
    }
    picRGB = alocaImagemRGB (5, 10);
    if (picRGB._img == NULL || picRGB.img == NULL){
        printf ("Erro ao alocar imagem RGB.\n\n");
        return 1;
    }
    char *fileName, *desc;
    fileName = (char*) calloc (30, sizeof(char));
    desc = (char*) calloc (80, sizeof(char));
    
    printf("Nome do arquivo (sem extensao): ");
    fgets (fileName, 30, stdin);
    fileName[strcspn(fileName, "\n")] = '\0';

    aux = loadHead (fileName, desc);

    if (aux == 0){
        printf ("Erro ao carregar cabecalho de imagem.\n\n");
    }
    else if (aux == 1){
        printf ("Imagem do tipo Gray.\n");
        printf ("Descricao: %sNome do arquivo: %s\n", desc, fileName);
    }
    else if (aux == 2){
        printf ("Imagem do tipo RGB.\n");
        printf ("Descricao: %sNome do arquivo: %s\n", desc, fileName);
    }

    printf ("Fim do programa.");
    free (fileName);
    free (desc);
    free (picRGB._img);
    free (picRGB.img);
    free (picgray._img);
    free (picgray.img);
    return 0;
}

imgGray alocaImagemGray (int nLin, int nCol){
    imgGray picture;
    picture.nCol = nCol;
    picture.nLin = nLin;
    picture._img = NULL;
    picture.img = NULL;
    picture._img = (uchar*) calloc (nLin * nCol, sizeof(uchar));
    picture.img = (uchar**) calloc (nLin, sizeof(uchar*));
    if (picture._img == NULL || picture.img == NULL){
        return picture;
    }
    for (int i = 0; i < nLin; i++){
        picture.img[i] = &picture._img[i * nCol];
    } 
    return picture;
}

imgRGB alocaImagemRGB (int nLin, int nCol){
    imgRGB pic;
    pic.nLin = nLin;
    pic.nCol = nCol;
    pic._img = NULL;
    pic .img = NULL;
    pic._img = (tRGB*) calloc (nLin * nCol, sizeof(tRGB));
    pic.img = (tRGB**) calloc (nLin, sizeof(tRGB*));
    if (pic._img == NULL || pic.img == NULL){
        return pic;
    }
    for (int i = 0; i < nLin; i++){
        pic.img[i] = &pic._img[i * nCol];
    }
    return pic;
}

int saveImgGray (imgGray img, char desc[], char fileName[]){
    char *fileNameB = (char*) calloc (30, sizeof(char));
    strcpy (fileNameB, fileName);
    strcat (fileName, ".hed");
    strcat (fileNameB, ".img");
    FILE *arqhed, *arqbin;
    arqhed = fopen (fileName, "w");
    if (arqhed){
        fprintf (arqhed, "Tipo da imagem: Gray\n");
        fprintf (arqhed, "Linhas: %d - Colunas: %d\n", img.nLin, img.nCol);
        fprintf (arqhed, "%s", desc);
        fprintf (arqhed, "Arquivo binario: %s", fileNameB);
        int aux = fclose (arqhed);
        if (aux){
            printf ("Erro ao fechar arquivo .hed.\n\n");
            free(fileNameB);
            return 0;
        }
    }
    else{
        perror ("Erro ao abrir arquivo .hed.\n\n");
        free(fileNameB);
        return 0;
    }

    arqbin = fopen (fileNameB, "wb");
    if (arqbin){
        fwrite (img._img, sizeof(uchar), img.nLin * img.nCol, arqbin);
        int aux = fclose (arqbin);
        if (aux){
            printf ("Erro ao fechar arquivo binario.\n\n");
            free(fileNameB);
            return 0;
        }
        free(fileNameB);
        return 1;
    }
    else{
        perror ("Erro ao abrir arquivo binario.\n\n");
        free(fileNameB);
        return 0;
    }
}

int saveImgRGB(imgRGB img, char desc[], char fileName[]){
    char *fileNameB = (char*) calloc (30, sizeof(char));
    strcpy (fileNameB, fileName);
    strcat (fileNameB, ".img");
    strcat(fileName, ".hed");
    FILE *arqbin, *arqhed;
    arqhed = fopen(fileName, "w");
    if (arqhed){
        fprintf (arqhed, "Tipo da imagem: RGB\n");
        fprintf (arqhed, "Linhas: %d - Colunas: %d\n", img.nLin, img.nCol);
        fprintf (arqhed, "%s", desc);
        fprintf (arqhed, "Arquivo binario: %s", fileNameB);
        int aux = fclose(arqhed);
        if (aux){
            printf ("Erro ao fechar arquivo .hed.\n\n");
            free(fileNameB);
            return 0;
        }
    }
    else{
        printf ("Erro ao abrir arquivo .hed.\n\n");
        free(fileNameB);
        return 0;
    }

    arqbin = fopen (fileNameB, "wb");
    if (arqbin){
        fwrite(img._img, sizeof(tRGB), img.nCol * img.nLin, arqbin);
        int aux = fclose(arqbin);
        if (aux){
            printf ("Erro ao fechar arquivo binario.\n\n");
            free (fileNameB);
            return 0;
        }
        free(fileNameB);
        return 1;
    }
    else{
        printf ("Erro ao abrir arquivo binario.\n\n");
        free (fileNameB);
        return 0;
    }

}

int loadHead(char fileName[], char desc[]){
    strcat(fileName, ".hed");
    FILE *arq;
    arq = fopen (fileName, "r");
    if (arq){
        fgets (desc, 80, arq);
        if (strcmp (desc, "Tipo da imagem: Gray\n") == 0){
            fgets(desc, 80, arq);
            fgets(desc, 80, arq);
            return 1;
        }
        else{
            fgets(desc, 80, arq);
            fgets(desc, 80, arq);
            return 2;
        }
    }
    else{
        printf ("Erro ao tentar abrir o arquivo.\n\n");
        return 0;
    }
}
