#include <stdio.h>
	
struct inicio{
	int h;
	int m;
	int s;
	int dec;
};
struct final{
	int h;
	int m;
	int s;
	int dec;
};
struct legendas{
	int n;
	struct inicio ini;
	struct final fim;
	char cont[100];
};

int verificatamanho(){
	int l=0, p=1, k, h1, m1, s1, sec1, h2, m2, s2, sec2;
	char c;
	FILE *legendas;
	
	legendas = fopen("legendas.txt", "r");
	if(legendas == NULL) printf("erro"); 
		
	c=getc(legendas);
	while(!feof(legendas)){
		c=getc(legendas);
		fscanf(legendas, "%d:%d:%d,%d --> %d:%d:%d,%d ", &h1, &m1, &s1, &sec1, &h2, &m2, &s2, &sec2);
		p=1;
		k=0;
		while(p){
			c=getc(legendas);
			if(c=='\n')k=1;
			else if(k==1 && (c>47 && c<58)) k=2;
			if(k==2 || feof(legendas)) p=0;
		}
			l++;
	}  
	fclose(legendas);	
	
	return l;
}
void escreve(struct legendas leg[], int ltot){
	int l, n=1;
	FILE *legendas;
	
	rename("legendas.txt", "legendas2.txt");
	legendas=fopen("legendas.txt", "w");
	remove("legendas2.txt");
	
	if(legendas==NULL){
		printf("erro"); return;
	}
	
	for(l=0; l<ltot; l++, n++){
		fprintf(legendas, "%d", n);
		fputs("\n", legendas);
		fprintf(legendas, "%02d:%02d:%02d,%03d --> %02d:%02d:%02d,%03d ", leg[l].ini.h, leg[l].ini.m, leg[l].ini.s, leg[l].ini.dec, leg[l].fim.h, leg[l].fim.m, leg[l].fim.s, leg[l].fim.dec);		
		fputs("\n", legendas);
		fprintf(legendas, leg[l].cont);
	}
	fclose(legendas);
} 

void exibearq(){	
	FILE *legendas;
	char c;
	legendas = fopen("legendas.txt", "r");
	printf("\nNOME DO ARQUIVO: legendas.txt\n");
	c=getc(legendas);
	while(!feof(legendas)){
		printf("%c", c);
		c=getc(legendas);
	}
	printf("\n\n");
	fclose(legendas);
}
void soma(struct legendas leg[], int ltot){
	int l, s, ms;
	
	printf("Digite quantos segundos e milissegundos deseja acrescentar no formato 00.000: ");
	scanf("%d.%d", &s, &ms);
	printf("Digite o numero da legenda que deseja atrasar: ");
	scanf("%d", &l);
	if(l==0||l>ltot){printf("ERRO: legenda nao existe"); return;} 
	l-=1;
	leg[l].ini.dec+=ms; if(leg[l].ini.dec>999){ leg[l].ini.dec=leg[l].ini.dec%1000; leg[l].ini.s+=1;}	
	leg[l].ini.s+=s;	if(leg[l].ini.s>60){ leg[l].ini.s=leg[l].ini.s%60; leg[l].ini.m+=1;}
	leg[l].fim.dec+=ms; if(leg[l].fim.dec>999){ leg[l].fim.dec=leg[l].fim.dec%1000; leg[l].fim.s+=1;}	
	leg[l].fim.s+=s;	if(leg[l].fim.s>60){ leg[l].fim.s=leg[l].fim.s%60; leg[l].fim.m+=1;}
	
	escreve(leg, ltot);
}
void subtrai(struct legendas leg[], int ltot){
	int l=0, seg, ms;
	
	printf("Digite quantos segundos e milissegundos deseja diminuir de todas as legendas no formato 00.000: ");
	scanf("%d.%d", &seg, &ms);
	
	for(l=0; l<ltot; l++){
		if(seg>leg[l].ini.s||(seg>=leg[l].ini.s && ms>leg[l].ini.dec)){printf("ERRO: o tempo e maior que o tempo da legenda %d\n", l+1); return;}
		leg[l].ini.s-=seg;	
	   	leg[l].ini.dec-=ms; 	
		leg[l].fim.s-=seg;	
	   	leg[l].fim.dec-=ms; 		  
	}
		
	escreve(leg, ltot);	
}
int elimina(struct legendas leg[], int ltot){
	int l, prim, ult, aux, dif, n=0, x=1, s;
	FILE *legendas;
	
	printf("Digite a primeira legenda do intervelo que deseja eliminar: ");
	scanf("%d", &prim);
	printf("Digite a ultima legenda do intervelo que deseja eliminar: ");
	scanf("%d", &ult);
	if(prim>ltot||ult==0){ printf("ERRO: legenda inexistente\n\n"); return 0;}
	if(ult>ltot||ult==0){ printf("ERRO: legenda inexistente\n\n"); return 0;}	
	if(ult==prim){printf("ERRO: Nao e um conjunto de legendas\n\n"); return 0;}
	if(prim>ult){aux=prim; prim=ult; ult=aux;}
	
	prim-=1;
	ult-=1;	
	dif=ult-prim; 
	ltot=ltot-dif-1;	
	
	rename("legendas.txt", "legendas2.txt");
	legendas=fopen("legendas.txt", "w");
	remove("legendas2.txt");
	
	if(legendas==NULL){
		printf("erro"); return 0;
	}
	
	for(n=0, l=0; l<ltot; l++, n++, x++){
		if(l>=prim && l<ult){ n=l+dif+1;}
		leg[n].n=x;
		fprintf(legendas, "%d", x);
		fputs("\n", legendas);
		leg[l].ini.h=leg[n].ini.h; leg[l].ini.m=leg[n].ini.m; leg[l].ini.s=leg[n].ini.s, leg[l].ini.dec=leg[n].ini.dec;
		leg[l].fim.h=leg[n].fim.h; leg[l].fim.m=leg[n].fim.m; leg[l].fim.s=leg[n].fim.s, leg[l].fim.dec=leg[n].fim.dec;
		fprintf(legendas, "%02d:%02d:%02d,%03d --> %02d:%02d:%02d,%03d ", leg[l].ini.h, leg[l].ini.m, leg[l].ini.s, leg[l].ini.dec, leg[l].fim.h, leg[l].fim.m, leg[l].fim.s, leg[l].fim.dec);		
		fputs("\n", legendas);
		for(s=0; leg[n].cont[s]; s++){
		leg[l].cont[s]=leg[n].cont[s];}
		leg[l].cont[s]='\0';
		fprintf(legendas, leg[l].cont);
	}
	
	fclose(legendas);
	printf("\n");
	
	return l;
}

int main(){
	FILE *legendas;
	struct legendas leg[verificatamanho()];
	char c;
	int l=0, x=0, p=1, n, ltot, k, s;
	
	legendas = fopen("legendas.txt", "r");
	if(legendas == NULL) printf("erro"); 
	
	while(!feof(legendas)){
		fscanf(legendas, "%d", &n);
		leg[l].n=n;
		fscanf(legendas, "%d:%d:%d,%d --> %d:%d:%d,%d ", &leg[l].ini.h, &leg[l].ini.m, &leg[l].ini.s, &leg[l].ini.dec, &leg[l].fim.h, &leg[l].fim.m, &leg[l].fim.s, &leg[l].fim.dec);		
		s=0;
		p=1;
		k=0;
		while(p){
			c=getc(legendas);
			if(c=='\n')k=1;
			else if(k==1 && (c>47 && c<58)) k=2;
			
			if(k==2){ leg[l].cont[s]='\0'; fseek(legendas, -1l, SEEK_CUR); p=0; }
			else if(feof(legendas)){leg[l].cont[s]='\0'; p=0;}
			else{
				leg[l].cont[s]=c; s++;
			}
		}
		l++;
	}  
	fclose(legendas);
	ltot=l;
		
	while(x!=5){
		printf("_________________________________________________\n");
		printf("|           **ESCOLHA UMA OPCAO**                |\n");
		printf("|                                                |\n");
		printf("|1-Exibir o arquivo                              |\n");
		printf("|2-Acrescentar tempo a uma legenda (atrasar)     |\n");
		printf("|3-Diminuir tempo de todas as legendas (adiantar)|\n");
		printf("|4-Eliminar um conjunto de legendas              |\n");
		printf("|5-SAIR                                          |\n");
		printf("|________________________________________________|\n");
		printf("|");
		scanf("%d", &x);	
		if(x==1) exibearq();
		else if(x==2){ soma(leg, ltot);}
		else if(x==3){ subtrai(leg, ltot);}
		else if(x==4){ ltot=elimina(leg, ltot);}
			
		fflush(stdin);
	}

	return 0;
}
