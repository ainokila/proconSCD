#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

	   
     	
	class Cola {

		private :

			static const int TAM = 10 ;
			int cola[TAM];
			int numElementos ;
		public:
			Cola(){

				numElementos = 0 ;

			}

			void aniade(int elemento){
				cola[numElementos]=elemento;
				numElementos++;
			}

			bool lleno(){

				bool solucion=false ; 
				if(numElementos==TAM){
					solucion = true;
				}
				return solucion;

			}
		
			int getElemento(){
				int solucion = -1;

				if(numElementos!=0){

					solucion = cola[0];
					for(int i = 1 ; i < numElementos ; i++ ){
						cola[i-1]=cola[i];
					}
				}else{
					cerr << "No puedes obtener elemento " ;
				}
				return solucion ;
			}
			int getNum(){
				return numElementos;
			}


	};

	//******************************************************************************************
	//DECLARACION VARIABLES GLOBALES:

	Cola cola;
	sem_t semaforo ;                   
	const unsigned num_items  = 40 ;

	//******************************************************************************************


	unsigned producir_dato()
	{
	  static int contador = 0 ;
	  cout << "producido: " << contador << endl << flush ;
	  return contador++ ;
	}
	// ---------------------------------------------------------------------

	void consumir_dato( int dato )
	{
		cout << "dato recibido: " << dato << endl ;
	}
	// ---------------------------------------------------------------------

	void * productor( void * )
	{   
		if(!cola.lleno()){

	 		 for( unsigned i = 0 ; i < num_items ; i++ )
	  		  { 
		

				int dato = producir_dato() ;
				cola.aniade(dato);
				sem_post(&semaforo);//Incremento 1 el valor del semaforo , ya que hemos leido 1 dato.
			  }
	   
	  	}

	  return NULL ;
	}
	// ---------------------------------------------------------------------

	void * consumidor( void * )
	{   
	  for( unsigned i = 0 ; i < num_items ; i++ )
	  {   
		int dato ;

		sem_wait(&semaforo);
		if(cola.getNum()!=0){
			dato=cola.getElemento();
		}

		consumir_dato( dato ) ;
	  }
	  return NULL ;
	}
	//----------------------------------------------------------------------

	int main()
	{

		pthread_t  productor;
		pthread_t  consumidor ; 
	
	 
		sem_init( &semaforo, 0, 0 );

		pthread_create(&productor,NULL,productor,NULL);
		pthread_create(&consumidor,NULL,consumidor,NULL);

		pthread_join( productor, NULL );
		pthread_join( consumidor, NULL );
			
		cout << "Fin de la tarea";

	   return 0 ; 
	}
