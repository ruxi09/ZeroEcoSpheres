//Declare any variables shared between functions here

#define NUM_DEBRIS 13

float vecTarget[3];

float otherState[12];

float myState[12];

float debris[NUM_DEBRIS][4];

float distMari[4];

float target[3];

float myPos[3];

float otherPos[3];

int step;

int negative;

int goingThroughDebris;

float bigDebrisRadius;

float halfFieldWidth;

float sphRadius;

float enoughRoom;

float otherY;

float startingYPos;

float maxi;

int indiceDistMax;

float speed[3];

float distance(float pos1[3], float pos2[3]) {

    float vecBet[3];

    mathVecSubtract(vecBet, pos2, pos1, 3);

    return mathVecMagnitude(vecBet, 3);

}



void init() {



	//This function is called once when your code is first loaded.

    game.getDebris(debris);

    bigDebrisRadius = 0.09;

    halfFieldWidth = 0.64;

    sphRadius = 0.11;

    enoughRoom = 0.25;

    otherY = -0.50;

    startingYPos = 0.7;



    for(int i = 10; i >= 8; i--) {

        for(int j = 0; j < 4; j++) {

            debris[i+1][j] = debris[i][j];



        }

    }



    debris[12][0] = halfFieldWidth + bigDebrisRadius + sphRadius;

    debris[12][1] = 0.00;

    debris[12][2] = 0.00;

    debris[12][3] = bigDebrisRadius;

    debris[8][0] = -(halfFieldWidth + bigDebrisRadius + sphRadius);

    debris[8][1] = 0.00;

    debris[8][2] = 0.00;

    debris[8][3] = bigDebrisRadius;





    /*for(int i = 0; i < NUM_DEBRIS; i++) {

        DEBUG(("Asteroid %d: %f %f %f %f", i, debris[i][0], debris[i][1], debris[i][2], debris[i][3]));

    }*/

    for(int i = 0; i < 4; i++) {

        distMari[i] = debris[i+9][0] - debris[i+8][0];

        distMari[i]-= bigDebrisRadius*2;

    }





    maxi = -1;

    step = 1;

    goingThroughDebris = 1;

    negative = 0;





    //DEBUG(("Distantele sunt: %f %f %f %f", distMari[0], distMari[1], distMari[2], distMari[3]));

	//IMPORTANT: make sure to set any variables that need an initial value.

	//Do not assume variables will be set to 0 automatically!

}







void loop() {



	//This function is called once per second.  Use it to control the satellite.

	api.getMyZRState(myState);

	for(int i = 0; i< 3 ; i++)

	    myPos[i] = myState[i];

	api.getOtherZRState(otherState);

	for(int i = 0; i< 3 ; i++)

	    otherPos[i] = otherState[i];



	mathVecSubtract(vecTarget, otherState, myPos, 3);

    mathVecNormalize(vecTarget,3);

    api.setAttitudeTarget(vecTarget);



    //DEBUG(("Stepul este %d",step));



	switch (step) {

	    case 1:

	        if(goingThroughDebris) {

	            for(int i = 0; i < 4; i++) {

	                if(distMari[i] > maxi) {

	                    maxi = distMari[i];

	                    indiceDistMax = i;

	                }

	            }

	            target[0] = (debris[indiceDistMax+9][0] + debris[indiceDistMax+8][0])/2;

	            target[1] = 0.00;

	            target[2] = 0.00;



	            target[0] = ((distMari[1] > enoughRoom) ? (debris[10][0] + debris[9][0])/2 : ((distMari[2] > enoughRoom) ? ((debris[10][0] + debris[11][0])/2) : (debris[indiceDistMax+9][0] + debris[indiceDistMax+8][0])/2));



	            if(target[0] < 0) {

	                negative = 1;

	            }

	            goingThroughDebris = 1;

	            step = 2;

	            //DEBUG(("Targetul este %f %f %f",target[0],target[1],target[2]));



	        }

	    break;



	    case 2:



	        target[1] = startingYPos;

	        api.setPositionTarget(target);

	        if(distance(myPos,target) < 0.07)   ///asta trebuie modificata(distanta)

	            step = 3;

	    break;





	    case 3:



	       api.setPositionTarget((float[]){myPos[0],otherY, 0.00});



	        if(myPos[1] < -0.495)    ///trebe ceva calcule la toate distantele in fct de viteze,pozitii idk

	            step = 4;







	        break;



	    case 4:





	       api.setPositionTarget(otherState);

	       if(negative == 0) {

	            if(distance(myPos,otherState) < 0.20)  ///asta trebuie modificata

	                api.setPositionTarget(myPos);

	        }

	        else {

	            if(distance(myPos,otherState) < 0.57) ///deci si asta trebe modificata

	                api.setPositionTarget(myPos);



	        }







	       if(game.checkRendezvous())

	            game.completeRendezvous();



	       break;





	}





}



