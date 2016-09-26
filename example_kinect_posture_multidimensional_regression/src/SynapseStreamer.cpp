#include "SynapseStreamer.h"

SynapseStreamer::SynapseStreamer(){
    sendMessageCounterValue = 30;
    synapseConnectionOpen = false;
    streamingConnectionOpen = false;
    streamJointPositions = false;
    computeHandDistanceFeature = false;
    trackAllJoints(true);
}

SynapseStreamer::~SynapseStreamer(){
    
}

void SynapseStreamer::openSynapseConnection(unsigned int receiverIncomingDataPort, unsigned int senderOutgoingDataPort,string ipAddress){
    receiver.setup( receiverIncomingDataPort );
    sender.setup( ipAddress, senderOutgoingDataPort );
    synapseConnectionOpen = true;
}

void SynapseStreamer::openOutgoingConnection(unsigned int outgoingDataPort,string ipAddress){
    streamer.setup( ipAddress, outgoingDataPort );
    streamingConnectionOpen = true;
    streamJointPositions = true;
}

void SynapseStreamer::drawTrackedJoints(float x, float y, float width, float height){
    ofPushMatrix();
    ofTranslate(x, y);
    
    ofPopMatrix();
}

/////////////////////////////////////// Getters ///////////////////////////////////////
bool SynapseStreamer::getNewMessage(){
    if( newMessageReceived ){
        newMessageReceived = false;
        return true;
    }
    return false;
}

/////////////////////////////////////// Setters ///////////////////////////////////////
void SynapseStreamer::trackAllJoints(bool trackStatus,unsigned int jointPos){
    clear();
    trackHead(trackStatus,jointPos);
    trackTorso(trackStatus,jointPos);
    trackRightShoulder(trackStatus,jointPos);
    trackLeftShoulder(trackStatus,jointPos);
    trackRightElbow(trackStatus,jointPos);
    trackLeftElbow(trackStatus,jointPos);
    trackRightHand(trackStatus,jointPos);
    trackLeftHand(trackStatus,jointPos);
    trackRightHip(trackStatus,jointPos);
    trackLeftHip(trackStatus,jointPos);
    trackRightKnee(trackStatus,jointPos);
    trackLeftKnee(trackStatus,jointPos);
    trackRightFoot(trackStatus,jointPos);
    trackLeftFoot(trackStatus,jointPos);
}

void SynapseStreamer::trackHead(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            head.trackBodyJoint = trackStatus;
            head.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            head.trackBodyJoint = false;
            head.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            head.trackBodyJoint = trackStatus;
            head.trackWorldJoint = false;
            break;
            default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::trackTorso(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            torso.trackBodyJoint = trackStatus;
            torso.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            torso.trackBodyJoint = false;
            torso.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            torso.trackBodyJoint = trackStatus;
            torso.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }

}

void SynapseStreamer::trackRightShoulder(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            rightShoulder.trackBodyJoint = trackStatus;
            rightShoulder.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            rightShoulder.trackBodyJoint = false;
            rightShoulder.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            rightShoulder.trackBodyJoint = trackStatus;
            rightShoulder.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::trackLeftShoulder(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            leftShoulder.trackBodyJoint = trackStatus;
            leftShoulder.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            leftShoulder.trackBodyJoint = false;
            leftShoulder.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            leftShoulder.trackBodyJoint = trackStatus;
            leftShoulder.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }

}

void SynapseStreamer::trackRightElbow(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            rightElbow.trackBodyJoint = trackStatus;
            rightElbow.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            rightElbow.trackBodyJoint = false;
            rightElbow.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            rightElbow.trackBodyJoint = trackStatus;
            rightElbow.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::trackLeftElbow(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            leftElbow.trackBodyJoint = trackStatus;
            leftElbow.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            leftElbow.trackBodyJoint = false;
            leftElbow.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            leftElbow.trackBodyJoint = trackStatus;
            leftElbow.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::trackRightHand(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            rightHand.trackBodyJoint = trackStatus;
            rightHand.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            rightHand.trackBodyJoint = false;
            rightHand.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            rightHand.trackBodyJoint = trackStatus;
            rightHand.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
    
}

void SynapseStreamer::trackLeftHand(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            leftHand.trackBodyJoint = trackStatus;
            leftHand.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            leftHand.trackBodyJoint = false;
            leftHand.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            leftHand.trackBodyJoint = trackStatus;
            leftHand.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::trackRightHip(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            rightHip.trackBodyJoint = trackStatus;
            rightHip.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            rightHip.trackBodyJoint = false;
            rightHip.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            rightHip.trackBodyJoint = trackStatus;
            rightHip.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::trackLeftHip(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            leftHip.trackBodyJoint = trackStatus;
            leftHip.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            leftHip.trackBodyJoint = false;
            leftHip.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            leftHip.trackBodyJoint = trackStatus;
            leftHip.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::trackRightKnee(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            rightKnee.trackBodyJoint = trackStatus;
            rightKnee.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            rightKnee.trackBodyJoint = false;
            rightKnee.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            rightKnee.trackBodyJoint = trackStatus;
            rightKnee.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::trackLeftKnee(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            rightKnee.trackBodyJoint = trackStatus;
            rightKnee.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            rightKnee.trackBodyJoint = false;
            rightKnee.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            rightKnee.trackBodyJoint = trackStatus;
            rightKnee.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::trackRightFoot(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            rightFoot.trackBodyJoint = trackStatus;
            rightFoot.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            rightFoot.trackBodyJoint = false;
            rightFoot.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            rightFoot.trackBodyJoint = trackStatus;
            rightFoot.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::trackLeftFoot(bool trackStatus,unsigned int jointPos){
    switch( jointPos ){
        case ALL_JOINT_POSITIONS:
            leftFoot.trackBodyJoint = trackStatus;
            leftFoot.trackWorldJoint = trackStatus;
            break;
        case WORLD_JOINT_POSITION:
            leftFoot.trackBodyJoint = false;
            leftFoot.trackWorldJoint = trackStatus;
            break;
        case BODY_JOINT_POSITION:
            leftFoot.trackBodyJoint = trackStatus;
            leftFoot.trackWorldJoint = false;
            break;
        default:
            cout << "ERROR: Unknown jointPos!\n";
            break;
    }
}

void SynapseStreamer::clear(){
    sendMessageCounter = 0;
    newMessageReceived = false;
    head.clear();
    torso.clear();
    rightShoulder.clear();
    leftShoulder.clear();
    rightElbow.clear();
    leftElbow.clear();
    rightHand.clear();
    leftHand.clear();
    rightHip.clear();
    leftHip.clear();
    rightKnee.clear();
    leftKnee.clear();
    rightFoot.clear();
    leftFoot.clear();

}

void SynapseStreamer::parseIncomingMessages(){
    
    if( !synapseConnectionOpen ) return;
    
    //Check to see if we need to ping the Synergy server to make sure it keeps sending us data
    if( ++sendMessageCounter >= sendMessageCounterValue ){
        sendMessageCounter = 0;
        sendJointRequests();
    }
    
    // check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
        m.getAddress();
        
        if( m.getAddress() == "/head_pos_body" )
		{
			head.localX = m.getArgAsFloat( 0 );
            head.localY = m.getArgAsFloat( 1 );
            head.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/head_pos_world" )
		{
			head.worldX = m.getArgAsFloat( 0 );
            head.worldY = m.getArgAsFloat( 1 );
            head.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/torso_pos_body" )
		{
			torso.localX = m.getArgAsFloat( 0 );
            torso.localY = m.getArgAsFloat( 1 );
            torso.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/torso_pos_world" )
		{
			torso.worldX = m.getArgAsFloat( 0 );
            torso.worldY = m.getArgAsFloat( 1 );
            torso.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/rightshoulder_pos_body" )
		{
			rightShoulder.localX = m.getArgAsFloat( 0 );
            rightShoulder.localY = m.getArgAsFloat( 1 );
            rightShoulder.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/rightshoulder_pos_world" )
		{
			rightShoulder.worldX = m.getArgAsFloat( 0 );
            rightShoulder.worldY = m.getArgAsFloat( 1 );
            rightShoulder.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/leftshoulder_pos_body" )
		{
			leftShoulder.localX = m.getArgAsFloat( 0 );
            leftShoulder.localY = m.getArgAsFloat( 1 );
            leftShoulder.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/leftshoulder_pos_world" )
		{
			leftShoulder.worldX = m.getArgAsFloat( 0 );
            leftShoulder.worldY = m.getArgAsFloat( 1 );
            leftShoulder.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/rightelbow_pos_body" )
		{
			rightElbow.localX = m.getArgAsFloat( 0 );
            rightElbow.localY = m.getArgAsFloat( 1 );
            rightElbow.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/rightelbow_pos_world" )
		{
			leftElbow.worldX = m.getArgAsFloat( 0 );
            leftElbow.worldY = m.getArgAsFloat( 1 );
            leftElbow.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/righthand_pos_body" )
		{
			rightHand.localX = m.getArgAsFloat( 0 );
            rightHand.localY = m.getArgAsFloat( 1 );
            rightHand.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/righthand_pos_world" )
		{
			rightHand.worldX = m.getArgAsFloat( 0 );
            rightHand.worldY = m.getArgAsFloat( 1 );
            rightHand.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/lefthand_pos_body" )
		{
			leftHand.localX = m.getArgAsFloat( 0 );
            leftHand.localY = m.getArgAsFloat( 1 );
            leftHand.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/lefthand_pos_world" )
		{
			leftHand.worldX = m.getArgAsFloat( 0 );
            leftHand.worldY = m.getArgAsFloat( 1 );
            leftHand.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/righthip_pos_body" )
		{
			rightHip.localX = m.getArgAsFloat( 0 );
            rightHip.localY = m.getArgAsFloat( 1 );
            rightHip.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/righthip_pos_world" )
		{
			rightHip.worldX = m.getArgAsFloat( 0 );
            rightHip.worldY = m.getArgAsFloat( 1 );
            rightHip.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/lefthip_pos_body" )
		{
			leftHip.localX = m.getArgAsFloat( 0 );
            leftHip.localY = m.getArgAsFloat( 1 );
            leftHip.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/lefthip_pos_world" )
		{
			leftHip.worldX = m.getArgAsFloat( 0 );
            leftHip.worldY = m.getArgAsFloat( 1 );
            leftHip.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/rightknee_pos_body" )
		{
			rightKnee.localX = m.getArgAsFloat( 0 );
            rightKnee.localY = m.getArgAsFloat( 1 );
            rightKnee.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/rightknee_pos_world" )
		{
			rightKnee.worldX = m.getArgAsFloat( 0 );
            rightKnee.worldY = m.getArgAsFloat( 1 );
            rightKnee.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/leftknee_pos_body" )
		{
			leftKnee.localX = m.getArgAsFloat( 0 );
            leftKnee.localY = m.getArgAsFloat( 1 );
            leftKnee.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/leftknee_pos_world" )
		{
			leftKnee.worldX = m.getArgAsFloat( 0 );
            leftKnee.worldY = m.getArgAsFloat( 1 );
            leftKnee.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/rightfoot_pos_body" )
		{
			rightFoot.localX = m.getArgAsFloat( 0 );
            rightFoot.localY = m.getArgAsFloat( 1 );
            rightFoot.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/rightfoot_pos_world" )
		{
			rightFoot.worldX = m.getArgAsFloat( 0 );
            rightFoot.worldY = m.getArgAsFloat( 1 );
            rightFoot.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/leftfoot_pos_body" )
		{
			leftFoot.localX = m.getArgAsFloat( 0 );
            leftFoot.localY = m.getArgAsFloat( 1 );
            leftFoot.localZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}else if( m.getAddress() == "/leftfoot_pos_world" )
		{
			leftFoot.worldX = m.getArgAsFloat( 0 );
            leftFoot.worldY = m.getArgAsFloat( 1 );
            leftFoot.worldZ = m.getArgAsFloat( 2 );
			newMessageReceived = true;
            //Pass the message on to the streamer
            if( streamJointPositions ) streamer.sendMessage( m );
		}

	}
    
    if( computeHandDistanceFeature ){
        handDistFeature = euclideanDistance(leftHand.getLocalCoordAsVector(), rightHand.getLocalCoordAsVector());
        if( streamJointPositions ){
            ofxOscMessage m;
            m.setAddress("/hand_distance_feature");
            m.addFloatArg(handDistFeature);
            streamer.sendMessage( m );
        }
    }

}

void SynapseStreamer::sendJointRequests(){
    
    ofxOscMessage m;

    if( head.trackBodyJoint ){
        m.clear();
        m.setAddress( "/head_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( head.trackWorldJoint ){
        m.clear();
        m.setAddress( "/head_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( torso.trackBodyJoint ){
        m.clear();
        m.setAddress( "/torso_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( torso.trackWorldJoint ){
        m.clear();
        m.setAddress( "/torso_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightShoulder.trackBodyJoint ){
        m.clear();
        m.setAddress( "/rightshoulder_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightShoulder.trackWorldJoint ){
        m.clear();
        m.setAddress( "/rightshoulder_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftShoulder.trackBodyJoint ){
        m.clear();
        m.setAddress( "/leftshoulder_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftShoulder.trackWorldJoint ){
        m.clear();
        m.setAddress( "/leftshoulder_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightElbow.trackBodyJoint ){
        m.clear();
        m.setAddress( "/rightelbow_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightElbow.trackWorldJoint ){
        m.clear();
        m.setAddress( "/rightelbow_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftElbow.trackBodyJoint ){
        m.clear();
        m.setAddress( "/leftelbow_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftElbow.trackWorldJoint ){
        m.clear();
        m.setAddress( "/leftelbow_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightHand.trackBodyJoint ){
        m.clear();
        m.setAddress( "/righthand_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightHand.trackWorldJoint ){
        m.clear();
        m.setAddress( "/righthand_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftHand.trackBodyJoint ){
        m.clear();
        m.setAddress( "/lefthand_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftHand.trackWorldJoint ){
        m.clear();
        m.setAddress( "/lefthand_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightHip.trackBodyJoint ){
        m.clear();
        m.setAddress( "/righthip_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightHip.trackWorldJoint ){
        m.clear();
        m.setAddress( "/righthip_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftHip.trackBodyJoint ){
        m.clear();
        m.setAddress( "/lefthip_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftHip.trackWorldJoint ){
        m.clear();
        m.setAddress( "/lefthip_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightKnee.trackBodyJoint ){
        m.clear();
        m.setAddress( "/rightknee_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightKnee.trackWorldJoint ){
        m.clear();
        m.setAddress( "/rightknee_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftKnee.trackBodyJoint ){
        m.clear();
        m.setAddress( "/leftknee_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftKnee.trackWorldJoint ){
        m.clear();
        m.setAddress( "/leftknee_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightFoot.trackBodyJoint ){
        m.clear();
        m.setAddress( "/rightfoot_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( rightFoot.trackWorldJoint ){
        m.clear();
        m.setAddress( "/rightfoot_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftFoot.trackBodyJoint ){
        m.clear();
        m.setAddress( "/leftfoot_trackjointpos" );
        m.addIntArg( BODY_JOINT_POSITION );
        sender.sendMessage( m );
    }
    if( leftFoot.trackWorldJoint ){
        m.clear();
        m.setAddress( "/leftfoot_trackjointpos" );
        m.addIntArg( WORLD_JOINT_POSITION );
        sender.sendMessage( m );
    }
}

double SynapseStreamer::euclideanDistance(vector< double > a,vector< double > b){
    double d = 0;
    if( a.size() != b.size() ) return 0;
    
    for(unsigned int i=0; i<a.size(); i++){
        d += (a[i]-b[i])*(a[i]-b[i]);
    }
    return sqrt( d );
}
