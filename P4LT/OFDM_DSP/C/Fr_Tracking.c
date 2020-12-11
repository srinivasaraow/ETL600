/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Fr_Tracking.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Frequency and clock offsets tracking
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/Fr_Tracking.c $
 * 
 * 11    24.07.08 15:26 Chhamaa
 * 
 * 6     5.04.06 15:28 Chhamaa
 * 
 * 6     17.03.06 14:41 Chhamaa
 * 
 * 4     16.09.05 9:41 Chalgyg
 * OFDM_DSP_V0_10 (1160905)
 * 
 * 2     26.04.05 8:42 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 7     22.12.04 9:30 Chchlee
 * 
 * 6     2.12.04 17:54 Chhamaa
 * 
 * 5     1.10.04 12:48 Leeb02
 * 
 * Initial version
 *       15.02.2003 DLK
 * Frequency compensation control variable in dependence of interpolation
 *       20.6.2004 PL
 * DS4000 control
 *		 26.09.04 PL
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/dctcxo/ad5321.h"
#include "../h/dctcxo/ad5321_defs.h"
#include "../h/dctcxo/ad5321_externs.h"

//#undef PHASECONTROL
#define PHASECONTROL

#define ALPHA_CO     0.007f //0.01f
#define ALPHA_PHASE  0.003f //0.005

#ifdef PHASECONTROL
// State Machine der Phasenregelung
#define PH_WAIT_FOR_MEASURE_REF_PHASE 	0
#define PH_MEASURE_REF_PHASE 	1
//#define PH_WAIT_FOR_ACTION 		2
#define PH_CONTROL_ACTION 		3
//#define PH_WAIT_FOR_FREQ_CTRL_1 4
//#define PH_WAIT_FOR_FREQ_CTRL_2 5

//#define PH_TEST_WAIT 			6
//#define PH_TEST_INJECT 			7

unsigned int PhaseControl_State_ctr;

float Freq_Boost_Factor;
float Delta_Freq_Boost_ppm;
float Phase_Delta;
float Phase_Delta_Diff;
float Phase_Delta_Boost;
float Clock_Ph_Offset_Tmp;
float Clock_Ph_Offset_Ref;
float Clock_Ph_Offset_old;
float Clock_Ph_Offset_new;
float Clock_Ph_Offset_Diff;
int AD5321_Phase_Control_Code;
int AD5321_Code_Zero;
int Phase_Control_Code_Delta;
unsigned int Num_Wait_LRs_halfhour;

//debug
unsigned int Phase_trend_ctr;
int segment ( "seg_extn SHT_NOBITS" ) Phase_trend_ControlCode[256];
float segment ( "seg_extn SHT_NOBITS" ) dbg_EPSILON_P[256];
float segment ( "seg_extn SHT_NOBITS" ) dbg_EPSILON_D[256];
int segment ( "seg_extn SHT_NOBITS" ) Phase_Code_Min;
int segment ( "seg_extn SHT_NOBITS" ) Phase_Code_Max;
int segment ( "seg_extn SHT_NOBITS" ) Phase_Code_Min_tot;
int segment ( "seg_extn SHT_NOBITS" ) Phase_Code_Max_tot;
int segment ( "seg_extn SHT_NOBITS" ) Phase_Code_Min_Map[168];// 1 Woche = 168h
int segment ( "seg_extn SHT_NOBITS" ) Phase_Code_Max_Map[168];// 1 Woche = 168h
unsigned int Phase_Code_minmax_ctr;
unsigned int Phase_Code_minmax_mapctr;

#endif //#ifdef PHASECONTROL

extern float SFC_Interpolation;
extern unsigned int F_trend_ctr;
// debug
float CO_delta;
float Alpha_CO; //initialized in OFDM_Freq_Estim.c



//========================================
void segment ( "seg_ext_pmco" ) Fr_Tracking()
	{
	int i;
	float CO_ppm;
	float FO_delta;
    
	if(Df_block_cnt == LEN_DF_EST_DATA) 
	{
	if (Level_Warning==0) // Adaption der Clock, nur
							// falls kein Pegelerror
		{	
		Len_Lin_Regr = LEN_DF_EST_DATA;

		// Linear regression for pilot 0
      	if(fabsf(Ph_Pil_t0[0] - Start_Ph_Pil0) > OFDM_PI)
      		{
      		ow0 -= OFDM_TWO_PI*fsignf(1.0f,Ph_Pil_t0[0] - Start_Ph_Pil0);
      		}

      	es1 = ow0 + Ph_Pil_t0[0];
   		ens1 = ow0 + Ph_Pil_t0[0];
   
   		for(i=1;i<Len_Lin_Regr;i++)
			{
      		if(fabsf(Ph_Pil_t0[i]-Ph_Pil_t0[i-1]) > OFDM_PI)
      			{
      			ow0 -= OFDM_TWO_PI*fsignf(1.0f,Ph_Pil_t0[i]-Ph_Pil_t0[i-1]);
      			}
      		es1 += ow0 + Ph_Pil_t0[i];
      		ens1 += (i+1)*(ow0+Ph_Pil_t0[i]);
      		}

		Fr_Pil0=(12.*ens1-6.*((float)Len_Lin_Regr+1.)*es1)/
   			(float)((Len_Lin_Regr+1)*(Len_Lin_Regr-1)*(float)NL*Len_Lin_Regr);

   		Ph_Clk0 = (2.0f*(2.0f*Len_Lin_Regr+1.0f)*es1-6.*ens1)/
             	  (float)(Len_Lin_Regr*(Len_Lin_Regr-1.0f));
		
        // Linear regression for pilot 1 	// Reference Pilot
      	if(fabsf(Ph_Pil_t1[0] - Start_Ph_Pil1) > OFDM_PI)
      		{
      		ow1 -= OFDM_TWO_PI*fsignf(1.0f,Ph_Pil_t1[0] - Start_Ph_Pil1);
      		}

		es1 = ow1 + Ph_Pil_t1[0];
   		ens1 = ow1 + Ph_Pil_t1[0];
   
   		for(i=1;i<Len_Lin_Regr;i++)
			{
      		if(fabsf(Ph_Pil_t1[i]-Ph_Pil_t1[i-1]) > OFDM_PI)
      			{
      			ow1 -= OFDM_TWO_PI*fsignf(1.0f,Ph_Pil_t1[i]-Ph_Pil_t1[i-1]);
      			}
      		es1 += ow1+Ph_Pil_t1[i];
      		ens1 += (i+1)*(ow1+Ph_Pil_t1[i]);
      		}

		Fr_Pil1=(12.*ens1-6.*((float)Len_Lin_Regr+1.)*es1)/
   			(float)((Len_Lin_Regr+1)*(Len_Lin_Regr-1)*(float)NL*Len_Lin_Regr);

   		Ph_Clk1 = (2.0f*(2.0f*Len_Lin_Regr+1.0f)*es1-6.*ens1)/
             	  (float)(Len_Lin_Regr*(Len_Lin_Regr-1.0f));
	
        // end of linear regression
        ow1 -=ow0; // parallel-Shift des 2-PI-Offsets auf Nulllinie
        ow0=0.0f;  // falls owx zu große Werte annimmt, dann liefert lineare Regression	
        		   // falsche, nämlich quantisierte Ergebnisse. Ich vermute weil dann gilt
        		   // Ph_Pil_tx[i] << owx und damit owx+Ph_Pil_tx[i] ~ owx
        		   // Durch diese Korrektur bleibt die Differenz des y-Achsenabschnitts der Phasenwerte
        		   // zwischen Pilot1 und Pilot2 erhalten, was für die Bestimmung der Momentanphase wichtig ist.
        		   
        
		//ow0=0.0f; //Diese Variante würde nicht funktionieren. Für Clockmessung zwar ausreichend,
		//ow1=0.0f;	// aber der y-Achsenabschnitt, für die Bestimmung der Phase, wäre mehrdeutig
					// Eine Messung mit absichtlicher Fehlstellung der Frequenz und damit wandernder Phase
					// hat das bestätigt. Es gibt Wrap-Around bei 2.0
		
     // On startup (or after Signal Interruption) skip first result
     // to get correct value of initial phase (Start_Ph_Pil0/1)
     if (Df_valid_result == 0)
     	{
     	Df_valid_result ++;
     	// skip result of linear regression, due to unknown Pilot Start Phase
     	}
     else
     	{	
     	Df_valid_result = 1;	
     	// use result of linear regression for frequency and clock control
     	
     	// -Estimate Frequency Offset with two independent sources
     	// -Frequency offset = 2pi*Fr_Offset_Hz/F_Nyquist_Hz
		// next operation is split into two steps
     	//		Freq_Offset += 0.1f*((float)Pilot_ind1*Fr_Pil0-
		//                             (float)Pilot_ind0*Fr_Pil1)/
		//                             (float)(Pilot_ind1-Pilot_ind0);
		FO_delta = ((float)Pilot_ind1*Fr_Pil0-
                             (float)Pilot_ind0*Fr_Pil1)/
                             (float)(Pilot_ind1-Pilot_ind0);
       
        // Kein I-Glied, da jetzt nur Steuerung                             
        // Freq_Offset +=  0.005f*FO_delta; 
        // Mittelung der Schätzungen des Frequenzversates
        Freq_Offset = Freq_Offset + 0.05f*(FO_delta - Freq_Offset); 
        
        // activate Frequency Offset Compensation 
        //Freq_Offset_Control_Flag = 1; //in SFC Filter              
        Frequency_Control_enable = 1;   // in OFDM_FreqCtrl_RX.c
        // -Estimate Clock Offset with two independent sources
     	// -Clock frequency offset = Clock_Offset_Hz/F_Nyquist_Hz
		// next operation is split into two steps
		//      Clock_Offset += Alpha_CO*(Fr_Pil0-Fr_Pil1)/(float)(Pilot_ind0-Pilot_ind1)/
		//                              OFDM_TWO_PI*(float)N;
        CO_delta = (Fr_Pil0-Fr_Pil1)/(float)(Pilot_ind0-Pilot_ind1)/
                              OFDM_TWO_PI*(float)N;
        
		//Alpha_CO=Alpha_CO*0.5f; if (Alpha_CO<0.05f) Alpha_CO = 0.05f; 
//        Alpha_CO = 0.001f; 

//debug inject Measurement Error
//CO_delta += 1.0E-8f; //Phase wandert nach links (Master-RX auf Ch1 Trigger, Slave-RX auf Ch2)
//CO_delta -= 0.240E-6f; // (Master-RX auf Ch1 Trigger, Slave-RX auf Ch2)
                                          
        Alpha_CO = ALPHA_CO;   // 0.01f                                        
        Clock_Offset += (Alpha_CO * CO_delta);
        
        
        // Clock phase offset = Delta_T_s*F_Nyquist_Hz
        Clock_Ph_Offset_Tmp = (Ph_Clk0-Ph_Clk1)/(float)(Pilot_ind0-Pilot_ind1)*(float)N/OFDM_TWO_PI;

        
		#ifdef PHASECONTROL
				// !! Debug-Switch zum Deaktivieren	
				//	if ( Modem_Config.phaseControlParam[0] > 0.0f) {
        	// State Machine der Phasenregelung
        	switch (Start_Clk_Ph) {
               	case PH_WAIT_FOR_MEASURE_REF_PHASE:
        			// warte 1/2h bis Frequenzregelung eingeschwungen, dann Referenzphase ermitteln	
//      		 		if (PhaseControl_State_ctr++ >= Num_Wait_LRs_halfhour) {
      		 		if (PhaseControl_State_ctr++ >= 256) {
   						PhaseControl_State_ctr = 0;
   						Start_Clk_Ph = PH_MEASURE_REF_PHASE;
        			}
        		break;
        	
        		case PH_MEASURE_REF_PHASE:
        			// Messung der Referenzphase
					Clock_Ph_Offset_Ref = Clock_Ph_Offset_Tmp;
					Clock_Ph_Offset_new = 0.0f;
					// Umrechnungsfaktor            Freq_Boost_Factor = 1.0E6f/( (float)((N+L)*Len_Lin_Regr)*1.2f ) ;
					// Stellgröße : N_LR*EPSILON_ppm = Clock_Ph_Offset_Diff /((N+L)*Len_Lin_Regr)
					//				N_LR = Anzahl der linearen Regressionen, bei denen Phasenspritze von Betrag EPSILON_ppm aktiv
					Freq_Boost_Factor = 1.0f/( (float)((N+L)*Len_Lin_Regr)*1.2f ) ; //nicht in PPM
					// Initialisierung des Counters für Debug-Messung über LongStatus
					Phase_trend_ctr=0; //debug
					// Rücksetzten des Inner-State-Counters
					PhaseControl_State_ctr = 0;
					// Mittenposition der VCTCXO-Kennlinie "Code=f(ppm)"
					AD5321_Code_Zero = AD5321_Convert_ppm2code(0.0f); //2076
					// Zustand für nächste LR
					Start_Clk_Ph = PH_CONTROL_ACTION;
	   			break;
   			
	   			case PH_CONTROL_ACTION: 
		   			// Phasenabweichung = aktuelle Phase - Referenzphase
   					Clock_Ph_Offset_old = Clock_Ph_Offset_new; 				   		// alte Phasenposition merken
   					Clock_Ph_Offset_new = Clock_Ph_Offset_Tmp-Clock_Ph_Offset_Ref;  // aktuelle Phasenposition
   					Clock_Ph_Offset_Diff = Clock_Ph_Offset_new - Clock_Ph_Offset_old; // Bewegung der Phase seit letzter Messung
   					/*-----------------------------
		   			Clock_Ph_Offset_Diff | Clock_Ph_Offset_new | Bewegung (*=Mitte, o=old, n=new)
		   			* o n	>0					>0					*->
		   			* n o	<0					>0					*<-
		   			o n *	>0					<0					->*
		   			n o *	<0					<0					<-*
		   			-------------------------------*/
   			
					// Berechnung der Stellgröße (Frequenzspritze)
					// P-Anteil, abhängig von aktueller Phasenposition in Bezug zu Referenzphase
					Phase_Delta = Clock_Ph_Offset_new * Freq_Boost_Factor; 		//=EPSILON_P (bei N_LR=1)
					// D-Anteil, abhängig von alter und neuer Phasenposition
					Phase_Delta_Diff = Clock_Ph_Offset_Diff * Freq_Boost_Factor;//=EPSILON_D (bei N_LR=1)
					// gewichtete Summe = PD-Anteil, ergibt Regelabweichung -> Stellgröße
					Phase_Delta_Boost = Phase_Delta + 0.5f*Phase_Delta_Diff;
			
					// Debug-Messungen
					dbg_EPSILON_P[Phase_trend_ctr] = Phase_Delta * 1.0e6f;
					dbg_EPSILON_D[Phase_trend_ctr] = Phase_Delta_Diff * 1.0e6f;
					Delta_Freq_Boost_ppm = Phase_Delta_Boost * 1.0e6f;
					// Umrechnung in Quantisierungsschritte für VCXO			
					AD5321_Phase_Control_Code = AD5321_Convert_ppm2code(Delta_Freq_Boost_ppm); 
					// Differenz zur Nullposition
            		Phase_Control_Code_Delta = - AD5321_Phase_Control_Code + AD5321_Code_Zero;
    				Phase_trend_ControlCode[Phase_trend_ctr]=Phase_Control_Code_Delta;
                	        
					// Debug, Min-Max-Protokoll
            		if (Phase_Control_Code_Delta < Phase_Code_Min ) Phase_Code_Min = Phase_Control_Code_Delta;
            		if (Phase_Control_Code_Delta > Phase_Code_Max ) Phase_Code_Max = Phase_Control_Code_Delta;
            		if (Phase_Code_minmax_ctr++ >=2*Num_Wait_LRs_halfhour ) { // ungefähr 1h rum 
						if (Phase_Code_minmax_mapctr < 168) { // für 168h Messung
            				Phase_Code_Min_Map[Phase_Code_minmax_mapctr]=Phase_Code_Min;
							Phase_Code_Max_Map[Phase_Code_minmax_mapctr]=Phase_Code_Max;
							Phase_Code_minmax_mapctr++;
            			}
            			if (Phase_Code_Min < Phase_Code_Min_tot) Phase_Code_Min_tot = Phase_Code_Min;
            			if (Phase_Code_Max > Phase_Code_Max_tot) Phase_Code_Max_tot = Phase_Code_Max;
            			Phase_Code_Min=0;
            			Phase_Code_Max=0;
            			Phase_Code_minmax_ctr=0;
           			}
            
           			
            
   					if (Phase_trend_ctr<255) {
   	             		Phase_trend_ctr++;
            		} else {
            			asm(" nop;");	
            			asm(" nop;");	
            			asm(" nop;");	
            		}
            		
				// Zustand für nächste LR            
            	Start_Clk_Ph = PH_CONTROL_ACTION;

            break;
            
        	}
        
        Clock_Offset += (ALPHA_PHASE * Phase_Delta_Boost);
//        Clock_Offset += (0.01f * Phase_Delta_Boost);
        //Clock_Offset += (0.01f * Phase_Delta + 0.005f * Phase_Delta_Diff);
        //} // vom Debug-Switch
        #endif
       //////////////////////////////////////// 
        
       
   		// Prepare DCTXCO control
        CO_ppm = Clock_Offset *(-1.0e6f);
        AD5321_Control_Code = AD5321_Convert_ppm2code(CO_ppm); 
        
        // Set control Flag to update AD5321 setting
        AD5321_Control_Flag = 1;                                             

		// Log result into buffer for debug
        // Frequency offset in Hz
        Df_Hz   =FO_delta    *(float)FN/OFDM_TWO_PI;
        Df_Hz_2 =Freq_Offset *(float)FN/OFDM_TWO_PI;

       	// Debug, save result into array
        if (F_trend_ctr < 1000)
        	{
        	F_trend_DF[F_trend_ctr]   = Df_Hz_2;
			F_trend_CO_delta[F_trend_ctr]   = CO_delta*1.0e6f;
			F_trend_CO[F_trend_ctr]   = Clock_Offset*1.0e6f;
			F_trend_CW[F_trend_ctr++] = AD5321_Control_Code;
        	}
        	

         // debug, to remove                      
  //       Freq_Offset = 0.0f;
        
     // -------------------- one common source , trial version -----------------------		
         /* 	             
           // Estimation of pure Clock Offset as common source
            // CO_i=N/2Pi * m_i / index_i
            CO_0 = Fr_Pil0 / (float)Pilot_ind0 /OFDM_TWO_PI*(float)N;   	// = T_receiver / T_transmitter
			CO_1 = Fr_Pil1 / (float)Pilot_ind1 /OFDM_TWO_PI*(float)N;	// = f_soll / f_ist
			
			//dt_rel0 = CO_0 - 1.0f;  // = delta_t / T_receiver
			//dt_rel1 = CO_1 - 1.0f;  // = delta_t / T_receiver
			//delta_f0_Hz = dt_rel0 * 16384000.0f; // delta_f in Hz   
			//delta_f1_Hz = dt_rel1 * 16384000.0f; // delta_f = f_soll - f_ist
			
			delta_f0_Hz = CO_0 * (float)FN; // delta_f in Hz   
			delta_f1_Hz = CO_1 * (float)FN; // delta_f = f_soll - f_ist
		*/	
	// ------------------------------------------------------------------------		
     
	
     	} // if (Df_valid_result == 0) else
     	
     	

 
        // Control variables :Rot_Fr.re , Rot_Fr.im, Delta_Phi_Symbol
        // are calculated directly in SFC_Filter_RX.c !!
        /*        	default:
			// Rotation coefficient for complex exponent to
			// frequency offset compensation; 
			// Control variable for "OFDM_SFC_RX.c"
			// 			considers SFC_Interpolation Factor
	       	Rot_Fr.re = cosf(Freq_Offset/SFC_Interpolation/2.0f+Freq_Offset_TX);
			Rot_Fr.im = -sinf(Freq_Offset/SFC_Interpolation/2.0f+Freq_Offset_TX);
	
			// Phase increment for one OFDM symbol
			// Control variable for "OFDM_SFC_RX.c"
			//			considers SFC_Interpolation Factor
			Delta_Phi_Symbol = -(Freq_Offset/SFC_Interpolation+Freq_Offset_TX*2.0f)*(float)NL;
	    	*/
		Start_Ph_Pil0 = Ph_Pil_t0[Len_Lin_Regr - 1];
		Start_Ph_Pil1 = Ph_Pil_t1[Len_Lin_Regr - 1];

		} // (Level_Warning==0)
		Df_block_cnt = 0;
	} // (Df_block_cnt == LEN_DF_EST_DATA) 
 }

#ifdef PHASECONTROL
/******************************************************************************
 // Initialisierung der Phase_Control_State_Machine
 // wird in Modem_Init_basic.c aufgerufen
 ********************************************************************/
void Phase_Control_Init(void) {
	int k;	
		PhaseControl_State_ctr = 0;
        Start_Clk_Ph = PH_WAIT_FOR_MEASURE_REF_PHASE;
        
        Phase_Delta 		= 0.0f;
        Phase_Delta_Diff 	= 0.0f;
        
        Clock_Ph_Offset_new	= 0.0f;
        Clock_Ph_Offset_old	= 0.0f;
        Clock_Ph_Offset_Ref = 0.0f;
        
        Phase_Delta_Boost = 0.0f;
            	
        
        // Debug Initialisierungen
        Phase_Code_Min = 0;
        Phase_Code_Max = 0;
        Phase_Code_Min_tot = 0;
		Phase_Code_Max_tot = 0;
		 for (k=0; k<168; k++) {
				Phase_Code_Min_Map[k]=0;
				Phase_Code_Max_Map[k]=0;
		 }
		Phase_Code_minmax_ctr = 0;
		Phase_Code_minmax_mapctr = 0;

   		//Num_Wait_LRs_halfhour = (unsigned int)(1800/24/TOFDM)
   		Num_Wait_LRs_halfhour =  (unsigned int)(1800.0f/(float)LEN_DF_EST_DATA/T_OFDM_symbol);
   			
}
#endif
