int main(void)
{
	struct packstruct {
		int a:1;
		int b:1;
		int c:3;
		int d:13;
	} pstruct;

	union gunion {
		struct packstruct p;
		int i;
	} ux;

	
	
	return 0;
}	
