int add(int a, int b){
	int res = a + b;
	return res;
}
int sub(int a, int b){
        int res = a - b;
        return res;
}
int mul(int a, int b){
        int res = a * b;
        return res;
}

int main(){
	int a;
	scanf("%d", &a);
	if(a>10){
		printf("%d",add(2, 3));
        }
	else if(a<9){
		printf("%d",sub(2, 3));
        }
	else{
		printf("%d", 10);
	}
	printf("%d",mul(2, 3));

	return 0;
}
