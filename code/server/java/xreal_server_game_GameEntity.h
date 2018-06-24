/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class xreal_server_game_GameEntity */

#ifndef _Included_xreal_server_game_GameEntity
#define _Included_xreal_server_game_GameEntity
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     xreal_server_game_GameEntity
 * Method:    allocateEntity0
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_allocateEntity0
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    freeEntity0
 * Signature:(I)Z
 */
JNIEXPORT jboolean JNICALL Java_xreal_server_game_GameEntity_freeEntity0
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    linkEntity0
 * Signature:(I)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_linkEntity0
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    unlinkEntity0
 * Signature:(I)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_unlinkEntity0
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_eType
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1eType
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_eType
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1eType
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_eFlags
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1eFlags
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_eFlags
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1eFlags
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_pos
 * Signature:(I)Lxreal / Trajectory;
 */
JNIEXPORT jobject JNICALL Java_xreal_server_game_GameEntity_getEntityState_1pos
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_pos
 * Signature:(IIIIFFFFFFFFF)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1pos
 (JNIEnv *, jclass, jint, jint, jint, jint, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_apos
 * Signature:(I)Lxreal / Trajectory;
 */
JNIEXPORT jobject JNICALL Java_xreal_server_game_GameEntity_getEntityState_1apos
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_apos
 * Signature:(IIIIFFFFFFFFF)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1apos
 (JNIEnv *, jclass, jint, jint, jint, jint, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_time
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1time
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_time
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1time
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_time2
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1time2
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_time2
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1time2
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_origin
 * Signature:(I)Ljavax / vecmath / Vector3f;
 */
JNIEXPORT jobject JNICALL Java_xreal_server_game_GameEntity_getEntityState_1origin
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_origin
 * Signature:(IFFF)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1origin
 (JNIEnv *, jclass, jint, jfloat, jfloat, jfloat);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_origin2
 * Signature:(I)Ljavax / vecmath / Vector3f;
 */
JNIEXPORT jobject JNICALL Java_xreal_server_game_GameEntity_getEntityState_1origin2
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_origin2
 * Signature:(IFFF)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1origin2
 (JNIEnv *, jclass, jint, jfloat, jfloat, jfloat);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_angles
 * Signature:(I)Lxreal / Angle3f;
 */
JNIEXPORT jobject JNICALL Java_xreal_server_game_GameEntity_getEntityState_1angles
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_angles
 * Signature:(IFFF)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1angles
 (JNIEnv *, jclass, jint, jfloat, jfloat, jfloat);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_angles2
 * Signature:(I)Lxreal / Angle3f;
 */
JNIEXPORT jobject JNICALL Java_xreal_server_game_GameEntity_getEntityState_1angles2
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_angles2
 * Signature:(IFFF)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1angles2
 (JNIEnv *, jclass, jint, jfloat, jfloat, jfloat);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_otherEntityNum
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1otherEntityNum
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_otherEntityNum
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1otherEntityNum
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_otherEntityNum2
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1otherEntityNum2
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_otherEntityNum2
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1otherEntityNum2
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_groundEntityNum
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1groundEntityNum
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_groundEntityNum
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1groundEntityNum
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_constantLight
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1constantLight
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_constantLight
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1constantLight
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_loopSound
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1loopSound
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_loopSound
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1loopSound
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_modelindex
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1modelindex
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_modelindex
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1modelindex
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_modelindex2
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1modelindex2
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_modelindex2
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1modelindex2
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_clientNum
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1clientNum
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_clientNum
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1clientNum
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_frame
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1frame
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_frame
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1frame
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_solid
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1solid
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_solid
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1solid
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_event
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1event
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_event
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1event
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_eventParm
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1eventParm
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_eventParm
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1eventParm
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_powerups
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1powerups
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_powerups
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1powerups
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_weapon
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1weapon
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_weapon
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1weapon
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_legsAnim
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1legsAnim
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_legsAnim
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1legsAnim
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_torsoAnim
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1torsoAnim
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_torsoAnim
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1torsoAnim
 (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    getEntityState_generic1
 * Signature:(I)I
 */
JNIEXPORT jint JNICALL Java_xreal_server_game_GameEntity_getEntityState_1generic1
 (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_server_game_GameEntity
 * Method:    setEntityState_generic1
 * Signature:(II)V
 */
JNIEXPORT void JNICALL Java_xreal_server_game_GameEntity_setEntityState_1generic1
 (JNIEnv *, jclass, jint, jint);

#ifdef __cplusplus
}
#endif
#endif