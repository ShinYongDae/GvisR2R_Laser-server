// EngraveDef.h

#pragma once
typedef struct stRcvSig
{
	// Normal
	BOOL _Idle, _Busy, _Connect;
	// Display
	BOOL _DispDualSample, _DispDualTest, _DispSingleSample, _DispSingleTest, _DispInitRun, _DispRun, _DispStop, _DispReady;
	// Dlg Info 
	BOOL _DualTest, _SampleTest, _TestMode, _RecoilerCcw, _UncoilerCcw, _AlignMethode,
		_DoorRecoiler, _DoorAoiUp, _DoorAoiDn, _DoorMk, _DoorEngrave, _DoorUncoiler, _SaftyMk, _CleannerAoiUp, _CleannerAoiDn,
		_UltraSonicAoiDn, _UltraSonicEngrave, _FixDef, _Use380mm, _DispContRun, _DispLotEnd;
	// Menu01
	BOOL _TempStop, _TempPause, _LotCut, _MkPosMv, _MkVerify, _ReelmapUp, _ReelmapDn, _ReelmapTot,
		_RemainMode, _EndWork, _ReMk, _2Layer, _1LnMk, _2LnMk, _3LnMk, _4LnMk, _TotLnMk, _UpdateWork, _DispDefImg;
	// Menu02
	BOOL _2DEngLenMvCw, _2DEngLenMvCcw, _2DAoiLenMvCw, _2DAoiLenMvCcw, _2DMkLenMvCw, _2DMkLenMvCcw, _2DOneShotLenCw, _2DOneShotLenCcw,
		_2DOffsetInitPos, _2DOffsetInitPosMove;
	// Menu03
	BOOL _Ready, _Run, _Reset, _Stop, _Auto, _OneCycle, _Manual, _MkTq, _AoiTq, _EngTq, _CcwModRe, _CcwModUn, _Core150Re, _Core150Un,
		_Relation, _JoinSelRe, _MvCwRe, _MvCcwRe, _PrdChuckRe, _DancerUpRe, _PasteUpLfRe, _PasteUpRtRe, _PasteVacRe, _PprChuckRe,
		_PprCwRe, _PprCcwRe, _DoRe, _PrdPprRe,
		_JoinSelMk, _MvCwMk, _MvCcwMk, _FdVacMk, _PushUpMk, _TblBlwMk, _TblVacMk, _FdClampMk, _TensClampMk, _OnePnlMk, _LsrMk,
		_DancerUpMk, _TqVacMk,
		_JoinSelAoiDn, _MvCwAoiDn, _MvCcwAoiDn, _FdVacAoiDn, _PushUpAoiDn, _TblBlwAoiDn, _TblVacAoiDn, _FdClampAoiDn, _TensClampAoiDn,
		_OnePnlAoiDn, _LsrAoiDn, _ClrRollAoiDn, _VelClrSonicAoiDn,
		_TestAoiDn, _ResetAoiDn, _LotEndAoiDn,
		_JoinSelAoiUp, _MvCwAoiUp, _MvCcwAoiUp, _FdVacAoiUp, _PushUpAoiUp, _TblBlwAoiUp, _TblVacAoiUp, _FdClampAoiUp, _TensClampAoiUp,
		_OnePnlAoiUp, _LsrAoiUp, _ClrRollAoiUp,
		_TestAoiUp, _ResetAoiUp, _LotEndAoiUp,
		_JoinSelEng, _MvCwEng, _MvCcwEng, _FdVacEng, _PushUpEng, _TblBlwEng, _TblVacEng, _FdClampEng, _TensClampEng, _OnePnlEng, _LsrEng,
		_DancerUpEng, _VelClrSonicEng,
		_JoinSelUn, _MvCwUn, _MvCcwUn, _PrdChuckUn, _DancerUpUn, _PasteUpLfUn, _PasteUpRtUn, _PasteVacUn, _PprChuckUn,
		_PprCwUn, _PprCcwUn, _ClrRollUpUn, _ClrRollPushUn,
		_EmgAoi;
	// Menu04
	BOOL _MkResetLf, _MkResetRt, _MkBuffJogCw, _MkBuffJogCcw, _MkBuffOrgMv, _MkBuffInitPosMv, _MkBuffPosSave;
	// Menu04 (Engrave)
	BOOL _EngBuffJogCw, _EngBuffJogCcw, _EngBuffOrgMv, _EngBuffOrgMvDone, _EngBuffInitPosMv, _EngBuffInitPosMvDone, _EngBuffInitPosSave;
	// EngraveAutoSequence
	BOOL _EngAutoInit, _EngAutoSeqMkSt, _EngAutoSeqOnMkIng, _EngAutoSeqMkDone, _EngAutoSeq2dReadSt, _EngAutoSeqOnReading2d, _EngAutoSeq2dReadDone, _EngAutoSeqFdDone;
	// DlgMyMsg
	BOOL _MyMsgYes, _MyMsgNo, _MyMsgCancel, _MyMsgOk;
	// Is Normal
	BOOL _IsIdle, _IsBusy, _IsConnect;
	// Is Display
	BOOL _IsDispDualSample, _IsDispDualTest, _IsDispSingleSample, _IsDispSingleTest, _IsDispInitRun, _IsDispRun, _IsDispStop, _IsDispReady;
	// Is Dlg Info 
	BOOL _IsDualTest, _IsSampleTest, _IsTestMode, _IsRecoilerCcw, _IsUncoilerCcw, _IsAlignMethode,
		_IsDoorRecoiler, _IsDoorAoiUp, _IsDoorAoiDn, _IsDoorMk, _IsDoorEngrave, _IsDoorUncoiler, _IsSaftyMk, _IsCleannerAoiUp, _IsCleannerAoiDn,
		_IsUltraSonicAoiDn, _IsUltraSonicEngrave, _IsFixDef;
	// Is Menu01
	BOOL _IsTempStop, _IsTempPause, _IsLotCut, _IsMkPosMv, _IsMkVerify, _IsReelmapUp, _IsReelmapDn, _IsReelmapTot,
		_IsRemainMode, _IsEndWork, _IsReMk, _Is2Layer, _Is1LnMk, _Is2LnMk, _Is3LnMk, _Is4LnMk, _IsTotLnMk, _IsUpdateWork;
	// Is Menu02
	BOOL _Is2DEngLenMvCw, _Is2DEngLenMvCcw, _Is2DAoiLenMvCw, _Is2DAoiLenMvCcw, _Is2DMkLenMvCw, _Is2DMkLenMvCcw, _Is2DOneShotLenCw, _Is2DOneShotLenCcw;
	// Is Menu03
	BOOL _IsReady, _IsRun, _IsReset, _IsStop, _IsAuto, _IsOneCycle, _IsManual, _IsMkTq, _IsAoiTq, _IsEngTq, _IsCcwModRe, _IsCcwModUn, _IsCore150Re, _IsCore150Un,
		_IsRelation, _IsJoinSelRe, _IsMvCwRe, _IsMvCcwRe, _IsPrdChuckRe, _IsDancerUpRe, _IsPasteUpLfRe, _IsPasteUpRtRe, _IsPasteVacRe, _IsPprChuckRe,
		_IsPprCwRe, _IsPprCcwRe, _IsDoRe, _IsPrdPprRe,
		_IsJoinSelMk, _IsMvCwMk, _IsMvCcwMk, _IsFdVacMk, _IsPushUpMk, _IsTblBlwMk, _IsTblVacMk, _IsFdClampMk, _IsTensClampMk, _IsOnePnlMk, _IsLsrMk,
		_IsDancerUpMk, _IsTqVacMk,
		_IsJoinSelAoiDn, _IsMvCwAoiDn, _IsMvCcwAoiDn, _IsFdVacAoiDn, _IsPushUpAoiDn, _IsTblBlwAoiDn, _IsTblVacAoiDn, _IsFdClampAoiDn, _IsTensClampAoiDn,
		_IsOnePnlAoiDn, _IsLsrAoiDn, _IsClrRollAoiDn, _IsVelClrSonicAoiDn,
		_IsTestAoiDn, _IsResetAoiDn, _IsLotEndAoiDn,
		_IsJoinSelAoiUp, _IsMvCwAoiUp, _IsMvCcwAoiUp, _IsFdVacAoiUp, _IsPushUpAoiUp, _IsTblBlwAoiUp, _IsTblVacAoiUp, _IsFdClampAoiUp, _IsTensClampAoiUp,
		_IsOnePnlAoiUp, _IsLsrAoiUp, _IsClrRollAoiUp,
		_IsTestAoiUp, _IsResetAoiUp, _IsLotEndAoiUp,
		_IsJoinSelEng, _IsMvCwEng, _IsMvCcwEng, _IsFdVacEng, _IsPushUpEng, _IsTblBlwEng, _IsTblVacEng, _IsFdClampEng, _IsTensClampEng, _IsOnePnlEng, _IsLsrEng,
		_IsDancerUpEng, _IsVelClrSonicEng,
		_IsJoinSelUn, _IsMvCwUn, _IsMvCcwUn, _IsPrdChuckUn, _IsDancerUpUn, _IsPasteUpLfUn, _IsPasteUpRtUn, _IsPasteVacUn, _IsPprChuckUn,
		_IsPprCwUn, _IsPprCcwUn, _IsClrRollUpUn, _IsClrRollPushUn,
		_IsEmgAoi;
	// Is Menu04
	BOOL _IsMkResetLf, _IsMkResetRt, _IsMkBuffJogCw, _IsMkBuffJogCcw, _IsMkBuffOrgMv, _IsMkBuffInitPosMv, _IsMkBuffPosSave;
	// Is Menu04 (Engrave)
	BOOL _IsEngBuffJogCw, _IsEngBuffJogCcw, _IsEngBuffOrgMv, _IsEngBuffOrgMvDone, _IsEngBuffInitPosMv, _IsEngBuffInitPosMvDone, _IsEngBuffInitPosSave;
	// Is EngraveAutoSequence
	BOOL _IsEngAutoInit, _IsEngAutoSeqMkSt, _IsEngAutoSeqOnMkIng, _IsEngAutoSeqMkDone, _IsEngAutoSeq2dReadSt, _IsEngAutoSeqOnReading2d, _IsEngAutoSeq2dReadDone;
	// Is DlgMyMsg
	BOOL _IsMyMsgYes, _IsMyMsgNo, _IsMyMsgCancel, _IsMyMsgOk;

	stRcvSig()
	{
		// Normal
		_Idle = FALSE; _Busy = FALSE; _Connect = FALSE;
		// Display
		_DispDualSample = FALSE; _DispDualTest = FALSE; _DispSingleSample = FALSE; _DispSingleTest = FALSE; _DispInitRun = FALSE; _DispRun = FALSE; _DispStop = FALSE; _DispReady = FALSE;
		// Dlg Info 
		_DualTest = FALSE; _SampleTest = FALSE; _TestMode = FALSE; _RecoilerCcw = FALSE; _UncoilerCcw = FALSE; _AlignMethode = FALSE;
		_DoorRecoiler = FALSE; _DoorAoiUp = FALSE; _DoorAoiDn = FALSE; _DoorMk = FALSE; _DoorEngrave = FALSE; _DoorUncoiler = FALSE; _SaftyMk = FALSE; _CleannerAoiUp = FALSE; _CleannerAoiDn = FALSE;
		_UltraSonicAoiDn = FALSE; _UltraSonicEngrave = FALSE; _FixDef = FALSE; _Use380mm = FALSE; _DispContRun = FALSE; _DispLotEnd = FALSE;
		// Menu01
		_TempStop = FALSE; _TempPause = FALSE; _LotCut = FALSE; _MkPosMv = FALSE; _MkVerify = FALSE; _ReelmapUp = FALSE; _ReelmapDn = FALSE; _ReelmapTot = FALSE;
		_RemainMode = FALSE; _EndWork = FALSE; _ReMk = FALSE; _2Layer = FALSE; _1LnMk = FALSE; _2LnMk = FALSE; _3LnMk = FALSE; _4LnMk = FALSE; _TotLnMk = FALSE; _UpdateWork = FALSE; _DispDefImg = FALSE;
		// Menu02
		_2DEngLenMvCw = FALSE; _2DEngLenMvCcw = FALSE; _2DAoiLenMvCw = FALSE; _2DAoiLenMvCcw = FALSE; _2DMkLenMvCw = FALSE; _2DMkLenMvCcw = FALSE; _2DOneShotLenCw = FALSE; _2DOneShotLenCcw = FALSE;
		_2DOffsetInitPos = FALSE; _2DOffsetInitPosMove = FALSE;
		// Menu03
		_Ready = FALSE; _Run = FALSE; _Reset = FALSE; _Stop = FALSE; _Auto = FALSE; _OneCycle = FALSE; _Manual = FALSE; _MkTq = FALSE; _AoiTq = FALSE; _EngTq = FALSE; _CcwModRe = FALSE; _CcwModUn = FALSE; _Core150Re = FALSE; _Core150Un = FALSE;
		_Relation = FALSE; _JoinSelRe = FALSE; _MvCwRe = FALSE; _MvCcwRe = FALSE; _PrdChuckRe = FALSE; _DancerUpRe = FALSE; _PasteUpLfRe = FALSE; _PasteUpRtRe = FALSE; _PasteVacRe = FALSE; _PprChuckRe = FALSE;
		_PprCwRe = FALSE; _PprCcwRe = FALSE; _DoRe = FALSE; _PrdPprRe = FALSE;
		_JoinSelMk = FALSE; _MvCwMk = FALSE; _MvCcwMk = FALSE; _FdVacMk = FALSE; _PushUpMk = FALSE; _TblBlwMk = FALSE; _TblVacMk = FALSE; _FdClampMk = FALSE; _TensClampMk = FALSE; _OnePnlMk = FALSE; _LsrMk = FALSE;
		_DancerUpMk = FALSE; _TqVacMk = FALSE;
		_JoinSelAoiDn = FALSE; _MvCwAoiDn = FALSE; _MvCcwAoiDn = FALSE; _FdVacAoiDn = FALSE; _PushUpAoiDn = FALSE; _TblBlwAoiDn = FALSE; _TblVacAoiDn = FALSE; _FdClampAoiDn = FALSE; _TensClampAoiDn = FALSE;
		_OnePnlAoiDn = FALSE; _LsrAoiDn = FALSE; _ClrRollAoiDn = FALSE; _VelClrSonicAoiDn = FALSE;
		_TestAoiDn = FALSE; _ResetAoiDn = FALSE; _LotEndAoiDn = FALSE;
		_JoinSelAoiUp = FALSE; _MvCwAoiUp = FALSE; _MvCcwAoiUp = FALSE; _FdVacAoiUp = FALSE; _PushUpAoiUp = FALSE; _TblBlwAoiUp = FALSE; _TblVacAoiUp = FALSE; _FdClampAoiUp = FALSE; _TensClampAoiUp = FALSE;
		_OnePnlAoiUp = FALSE; _LsrAoiUp = FALSE; _ClrRollAoiUp = FALSE;
		_TestAoiUp = FALSE; _ResetAoiUp = FALSE; _LotEndAoiUp = FALSE;
		_JoinSelEng = FALSE; _MvCwEng = FALSE; _MvCcwEng = FALSE; _FdVacEng = FALSE; _PushUpEng = FALSE; _TblBlwEng = FALSE; _TblVacEng = FALSE; _FdClampEng = FALSE; _TensClampEng = FALSE; _OnePnlEng = FALSE; _LsrEng = FALSE;
		_DancerUpEng = FALSE; _VelClrSonicEng = FALSE;
		_JoinSelUn = FALSE; _MvCwUn = FALSE; _MvCcwUn = FALSE; _PrdChuckUn = FALSE; _DancerUpUn = FALSE; _PasteUpLfUn = FALSE; _PasteUpRtUn = FALSE; _PasteVacUn = FALSE; _PprChuckUn = FALSE;
		_PprCwUn = FALSE; _PprCcwUn = FALSE; _ClrRollUpUn = FALSE; _ClrRollPushUn = FALSE;
		_EmgAoi = FALSE;
		// Menu04
		_MkResetLf = FALSE; _MkResetRt = FALSE; _MkBuffJogCw = FALSE; _MkBuffJogCcw = FALSE; _MkBuffOrgMv = FALSE; _MkBuffInitPosMv = FALSE; _MkBuffPosSave = FALSE;
		// Menu04 (Engrave)
		_EngBuffJogCw = FALSE; _EngBuffJogCcw = FALSE; _EngBuffOrgMv = FALSE; _EngBuffOrgMvDone = FALSE; _EngBuffInitPosMv = FALSE; _EngBuffInitPosMvDone = FALSE; _EngBuffInitPosSave = FALSE;
		// EngraveAutoSequence
		_EngAutoInit = FALSE; _EngAutoSeqMkSt = FALSE; _EngAutoSeqOnMkIng = FALSE; _EngAutoSeqMkDone = FALSE; _EngAutoSeq2dReadSt = FALSE; _EngAutoSeqOnReading2d = FALSE; _EngAutoSeq2dReadDone = FALSE; _EngAutoSeqFdDone = FALSE;
		// DlgMyMsg
		_MyMsgYes = FALSE; _MyMsgNo = FALSE; _MyMsgCancel = FALSE; _MyMsgOk = FALSE;
		// Is Normal
		_IsIdle = FALSE; _IsBusy = FALSE; _IsConnect = FALSE;
		// Is Display
		_IsDispDualSample = FALSE; _IsDispDualTest = FALSE; _IsDispSingleSample = FALSE; _IsDispSingleTest = FALSE; _IsDispInitRun = FALSE; _IsDispRun = FALSE; _IsDispStop = FALSE; _IsDispReady = FALSE;
		// Is Dlg Info 
		_IsDualTest = FALSE; _IsSampleTest = FALSE; _IsTestMode = FALSE; _IsRecoilerCcw = FALSE; _IsUncoilerCcw = FALSE; _IsAlignMethode = FALSE;
		_IsDoorRecoiler = FALSE; _IsDoorAoiUp = FALSE; _IsDoorAoiDn = FALSE; _IsDoorMk = FALSE; _IsDoorEngrave = FALSE; _IsDoorUncoiler = FALSE; _IsSaftyMk = FALSE; _IsCleannerAoiUp = FALSE; _IsCleannerAoiDn = FALSE;
		_IsUltraSonicAoiDn = FALSE; _IsUltraSonicEngrave = FALSE; _IsFixDef = FALSE;
		// Is Menu01
		_IsTempStop = FALSE; _IsTempPause = FALSE; _IsLotCut = FALSE; _IsMkPosMv = FALSE; _IsMkVerify = FALSE; _IsReelmapUp = FALSE; _IsReelmapDn = FALSE; _IsReelmapTot = FALSE;
		_IsRemainMode = FALSE; _IsEndWork = FALSE; _IsReMk = FALSE; _Is2Layer = FALSE; _Is1LnMk = FALSE; _Is2LnMk = FALSE; _Is3LnMk = FALSE; _Is4LnMk = FALSE; _IsTotLnMk = FALSE; _IsUpdateWork = FALSE;
		// Is Menu02
		_Is2DEngLenMvCw = FALSE; _Is2DEngLenMvCcw = FALSE; _Is2DAoiLenMvCw = FALSE; _Is2DAoiLenMvCcw = FALSE; _Is2DMkLenMvCw = FALSE; _Is2DMkLenMvCcw = FALSE; _Is2DOneShotLenCw = FALSE; _Is2DOneShotLenCcw = FALSE;
		// Is Menu03
		_IsReady = FALSE; _IsRun = FALSE; _IsReset = FALSE; _IsStop = FALSE; _IsAuto = FALSE; _IsOneCycle = FALSE; _IsManual = FALSE; _IsMkTq = FALSE; _IsAoiTq = FALSE; _IsEngTq = FALSE; _IsCcwModRe = FALSE; _IsCcwModUn = FALSE; _IsCore150Re = FALSE; _IsCore150Un = FALSE;
		_IsRelation = FALSE; _IsJoinSelRe = FALSE; _IsMvCwRe = FALSE; _IsMvCcwRe = FALSE; _IsPrdChuckRe = FALSE; _IsDancerUpRe = FALSE; _IsPasteUpLfRe = FALSE; _IsPasteUpRtRe = FALSE; _IsPasteVacRe = FALSE; _IsPprChuckRe = FALSE;
		_IsPprCwRe = FALSE; _IsPprCcwRe = FALSE; _IsDoRe = FALSE; _IsPrdPprRe = FALSE;
		_IsJoinSelMk = FALSE; _IsMvCwMk = FALSE; _IsMvCcwMk = FALSE; _IsFdVacMk = FALSE; _IsPushUpMk = FALSE; _IsTblBlwMk = FALSE; _IsTblVacMk = FALSE; _IsFdClampMk = FALSE; _IsTensClampMk = FALSE; _IsOnePnlMk = FALSE; _IsLsrMk = FALSE;
		_IsDancerUpMk = FALSE; _IsTqVacMk = FALSE;
		_IsJoinSelAoiDn = FALSE; _IsMvCwAoiDn = FALSE; _IsMvCcwAoiDn = FALSE; _IsFdVacAoiDn = FALSE; _IsPushUpAoiDn = FALSE; _IsTblBlwAoiDn = FALSE; _IsTblVacAoiDn = FALSE; _IsFdClampAoiDn = FALSE; _IsTensClampAoiDn = FALSE;
		_IsOnePnlAoiDn = FALSE; _IsLsrAoiDn = FALSE; _IsClrRollAoiDn = FALSE; _IsVelClrSonicAoiDn = FALSE;
		_IsTestAoiDn = FALSE; _IsResetAoiDn = FALSE; _IsLotEndAoiDn = FALSE;
		_IsJoinSelAoiUp = FALSE; _IsMvCwAoiUp = FALSE; _IsMvCcwAoiUp = FALSE; _IsFdVacAoiUp = FALSE; _IsPushUpAoiUp = FALSE; _IsTblBlwAoiUp = FALSE; _IsTblVacAoiUp = FALSE; _IsFdClampAoiUp = FALSE; _IsTensClampAoiUp = FALSE;
		_IsOnePnlAoiUp = FALSE; _IsLsrAoiUp = FALSE; _IsClrRollAoiUp = FALSE;
		_IsTestAoiUp = FALSE; _IsResetAoiUp = FALSE; _IsLotEndAoiUp = FALSE;
		_IsJoinSelEng = FALSE; _IsMvCwEng = FALSE; _IsMvCcwEng = FALSE; _IsFdVacEng = FALSE; _IsPushUpEng = FALSE; _IsTblBlwEng = FALSE; _IsTblVacEng = FALSE; _IsFdClampEng = FALSE; _IsTensClampEng = FALSE; _IsOnePnlEng = FALSE; _IsLsrEng = FALSE;
		_IsDancerUpEng = FALSE; _IsVelClrSonicEng = FALSE;
		_IsJoinSelUn = FALSE; _IsMvCwUn = FALSE; _IsMvCcwUn = FALSE; _IsPrdChuckUn = FALSE; _IsDancerUpUn = FALSE; _IsPasteUpLfUn = FALSE; _IsPasteUpRtUn = FALSE; _IsPasteVacUn = FALSE; _IsPprChuckUn = FALSE;
		_IsPprCwUn = FALSE; _IsPprCcwUn = FALSE; _IsClrRollUpUn = FALSE; _IsClrRollPushUn = FALSE;
		_IsEmgAoi = FALSE;
		// Is Menu04
		_IsMkResetLf = FALSE; _IsMkResetRt = FALSE; _IsMkBuffJogCw = FALSE; _IsMkBuffJogCcw = FALSE; _IsMkBuffOrgMv = FALSE; _IsMkBuffInitPosMv = FALSE; _IsMkBuffPosSave = FALSE;
		// Is Menu04 (Engrave)
		_IsEngBuffJogCw = FALSE; _IsEngBuffJogCcw = FALSE; _IsEngBuffOrgMv = FALSE; _IsEngBuffOrgMvDone = FALSE; _IsEngBuffInitPosMv = FALSE; _IsEngBuffInitPosMvDone = FALSE; _IsEngBuffInitPosSave = FALSE;
		// Is EngraveAutoSequence
		_IsEngAutoInit = FALSE; _IsEngAutoSeqMkSt = FALSE; _IsEngAutoSeqOnMkIng = FALSE; _IsEngAutoSeqMkDone = FALSE; _IsEngAutoSeq2dReadSt = FALSE; _IsEngAutoSeqOnReading2d = FALSE; _IsEngAutoSeq2dReadDone = FALSE;
		// Is DlgMyMsg
		_IsMyMsgYes = FALSE; _IsMyMsgNo = FALSE; _IsMyMsgCancel = FALSE; _IsMyMsgOk = FALSE;
	}

}RcvSig;

typedef enum _stPcId {
	_Engrave = 0, _AoiUp, _AoiDn, _Punch, _Vs,
	_IsEngrave, _IsAoiUp, _IsAoiDn, _IsPunch, _IsVs,
}_PcId;

typedef enum _stCmdCode {
	_None_ = 0, _GetSig, _SetSig, _GetData, _SetData, _Rtn, _Status,
	_IsNone_, _IsGetSig, _IsSetSig, _IsGetData, _IsSetData, _IsRtn, _IsStatus,
}_CmdCode;

typedef enum _stSigInx {
	// Normal
	_Idle = 0, _Busy, _Connect,
	// Display
	_DispDualSample, _DispDualTest, _DispSingleSample, _DispSingleTest, _DispInitRun, _DispRun, _DispStop, _DispReady,
	// Dlg Info 
	_DualTest, _SampleTest, _TestMode, _RecoilerCcw, _UncoilerCcw, _AlignMethode,
	_DoorRecoiler, _DoorAoiUp, _DoorAoiDn, _DoorMk, _DoorEngrave, _DoorUncoiler, _SaftyMk, _CleannerAoiUp, _CleannerAoiDn, 
	_UltraSonicAoiDn, _UltraSonicEngrave, _FixDef, _Use380mm, _DispContRun, _DispLotEnd,
	// Menu01
	_TempStop, _TempPause, _LotCut, _MkPosMv, _MkVerify, _ReelmapUp, _ReelmapDn, _ReelmapTot,
	_RemainMode, _EndWork, _ReMk, _2Layer, _1LnMk, _2LnMk, _3LnMk, _4LnMk, _TotLnMk, _UpdateWork, _DispDefImg,
	// Menu02
	_2DEngLenMvCw, _2DEngLenMvCcw, _2DAoiLenMvCw, _2DAoiLenMvCcw, _2DMkLenMvCw, _2DMkLenMvCcw, _2DOneShotLenCw, _2DOneShotLenCcw,
	_2DOffsetInitPos, _2DOffsetInitPosMove,
	// Menu03
	_Ready, _Run, _Reset, _Stop, _Auto, _OneCycle, _Manual, _MkTq, _AoiTq, _EngTq, _CcwModRe, _CcwModUn, _Core150Re, _Core150Un, 
	_Relation, _JoinSelRe, _MvCwRe, _MvCcwRe, _PrdChuckRe, _DancerUpRe, _PasteUpLfRe, _PasteUpRtRe, _PasteVacRe, _PprChuckRe,
	_PprCwRe, _PprCcwRe, _DoRe, _PrdPprRe,
	_JoinSelMk, _MvCwMk, _MvCcwMk, _FdVacMk, _PushUpMk, _TblBlwMk, _TblVacMk, _FdClampMk, _TensClampMk, _OnePnlMk, _LsrMk,
	_DancerUpMk, _TqVacMk,
	_JoinSelAoiDn, _MvCwAoiDn, _MvCcwAoiDn, _FdVacAoiDn, _PushUpAoiDn, _TblBlwAoiDn, _TblVacAoiDn, _FdClampAoiDn, _TensClampAoiDn, 
	_OnePnlAoiDn, _LsrAoiDn, _ClrRollAoiDn, _VelClrSonicAoiDn,
	_TestAoiDn, _ResetAoiDn, _LotEndAoiDn,
	_JoinSelAoiUp, _MvCwAoiUp, _MvCcwAoiUp, _FdVacAoiUp, _PushUpAoiUp, _TblBlwAoiUp, _TblVacAoiUp, _FdClampAoiUp, _TensClampAoiUp,
	_OnePnlAoiUp, _LsrAoiUp, _ClrRollAoiUp,
	_TestAoiUp, _ResetAoiUp, _LotEndAoiUp,
	_JoinSelEng, _MvCwEng, _MvCcwEng, _FdVacEng, _PushUpEng, _TblBlwEng, _TblVacEng, _FdClampEng, _TensClampEng, _OnePnlEng, _LsrEng,
	_DancerUpEng, _VelClrSonicEng,
	_JoinSelUn, _MvCwUn, _MvCcwUn, _PrdChuckUn, _DancerUpUn, _PasteUpLfUn, _PasteUpRtUn, _PasteVacUn, _PprChuckUn,
	_PprCwUn, _PprCcwUn, _ClrRollUpUn, _ClrRollPushUn,
	_EmgAoi,
	// Menu04
	_MkResetLf, _MkResetRt, _MkBuffJogCw, _MkBuffJogCcw, _MkBuffOrgMv, _MkBuffInitPosMv, _MkBuffPosSave,
	// Menu04 (Engrave)
	_EngBuffJogCw, _EngBuffJogCcw, _EngBuffOrgMv, _EngBuffOrgMvDone, _EngBuffInitPosMv, _EngBuffInitPosMvDone, _EngBuffInitPosSave,
	// EngraveAutoSequence
	_EngAutoInit, _EngAutoSeqMkSt, _EngAutoSeqOnMkIng, _EngAutoSeqMkDone, _EngAutoSeq2dReadSt, _EngAutoSeqOnReading2d, _EngAutoSeq2dReadDone, _EngAutoSeqFdDone,
	// DlgMyMsg
	_MyMsgYes, _MyMsgNo, _MyMsgCancel, _MyMsgOk,
	// PLC
	_Buzzer, _TowerLamp, _ErrorRead2dCode,
	// Is Normal
	_IsIdle, _IsBusy, _IsConnect,
	// Is Display
	_IsDispDualSample, _IsDispDualTest, _IsDispSingleSample, _IsDispSingleTest, _IsDispInitRun, _IsDispRun, _IsDispStop, _IsDispReady,
	// Is Dlg Info 
	_IsDualTest, _IsSampleTest, _IsTestMode, _IsRecoilerCcw, _IsUncoilerCcw, _IsAlignMethode,
	_IsDoorRecoiler, _IsDoorAoiUp, _IsDoorAoiDn, _IsDoorMk, _IsDoorEngrave, _IsDoorUncoiler, _IsSaftyMk, _IsCleannerAoiUp, _IsCleannerAoiDn,
	_IsUltraSonicAoiDn, _IsUltraSonicEngrave, _IsFixDef,
	// Is Menu01
	_IsTempStop, _IsTempPause, _IsLotCut, _IsMkPosMv, _IsMkVerify, _IsReelmapUp, _IsReelmapDn, _IsReelmapTot,
	_IsRemainMode, _IsEndWork, _IsReMk, _Is2Layer, _Is1LnMk, _Is2LnMk, _Is3LnMk, _Is4LnMk, _IsTotLnMk, _IsUpdateWork,
	// Is Menu02
	_Is2DEngLenMvCw, _Is2DEngLenMvCcw, _Is2DAoiLenMvCw, _Is2DAoiLenMvCcw, _Is2DMkLenMvCw, _Is2DMkLenMvCcw, _Is2DOneShotLenCw, _Is2DOneShotLenCcw,
	// Is Menu03
	_IsReady, _IsRun, _IsReset, _IsStop, _IsAuto, _IsOneCycle, _IsManual, _IsMkTq, _IsAoiTq, _IsEngTq, _IsCcwModRe, _IsCcwModUn, _IsCore150Re, _IsCore150Un,
	_IsRelation, _IsJoinSelRe, _IsMvCwRe, _IsMvCcwRe, _IsPrdChuckRe, _IsDancerUpRe, _IsPasteUpLfRe, _IsPasteUpRtRe, _IsPasteVacRe, _IsPprChuckRe,
	_IsPprCwRe, _IsPprCcwRe, _IsDoRe, _IsPrdPprRe,
	_IsJoinSelMk, _IsMvCwMk, _IsMvCcwMk, _IsFdVacMk, _IsPushUpMk, _IsTblBlwMk, _IsTblVacMk, _IsFdClampMk, _IsTensClampMk, _IsOnePnlMk, _IsLsrMk,
	_IsDancerUpMk, _IsTqVacMk,
	_IsJoinSelAoiDn, _IsMvCwAoiDn, _IsMvCcwAoiDn, _IsFdVacAoiDn, _IsPushUpAoiDn, _IsTblBlwAoiDn, _IsTblVacAoiDn, _IsFdClampAoiDn, _IsTensClampAoiDn,
	_IsOnePnlAoiDn, _IsLsrAoiDn, _IsClrRollAoiDn, _IsVelClrSonicAoiDn,
	_IsTestAoiDn, _IsResetAoiDn, _IsLotEndAoiDn,
	_IsJoinSelAoiUp, _IsMvCwAoiUp, _IsMvCcwAoiUp, _IsFdVacAoiUp, _IsPushUpAoiUp, _IsTblBlwAoiUp, _IsTblVacAoiUp, _IsFdClampAoiUp, _IsTensClampAoiUp,
	_IsOnePnlAoiUp, _IsLsrAoiUp, _IsClrRollAoiUp,
	_IsTestAoiUp, _IsResetAoiUp, _IsLotEndAoiUp,
	_IsJoinSelEng, _IsMvCwEng, _IsMvCcwEng, _IsFdVacEng, _IsPushUpEng, _IsTblBlwEng, _IsTblVacEng, _IsFdClampEng, _IsTensClampEng, _IsOnePnlEng, _IsLsrEng,
	_IsDancerUpEng, _IsVelClrSonicEng,
	_IsJoinSelUn, _IsMvCwUn, _IsMvCcwUn, _IsPrdChuckUn, _IsDancerUpUn, _IsPasteUpLfUn, _IsPasteUpRtUn, _IsPasteVacUn, _IsPprChuckUn,
	_IsPprCwUn, _IsPprCcwUn, _IsClrRollUpUn, _IsClrRollPushUn,
	_IsEmgAoi,
	// Is Menu04
	_IsMkResetLf, _IsMkResetRt, _IsMkBuffJogCw, _IsMkBuffJogCcw, _IsMkBuffOrgMv, _IsMkBuffInitPosMv, _IsMkBuffPosSave,
	// Is Menu04 (Engrave)
	_IsEngBuffJogCw, _IsEngBuffJogCcw, _IsEngBuffOrgMv, _IsEngBuffOrgMvDone, _IsEngBuffInitPosMv, _IsEngBuffInitPosMvDone, _IsEngBuffInitPosSave,
	// Is EngraveAutoSequence
	_IsEngAutoInit, _IsEngAutoSeqMkSt, _IsEngAutoSeqOnMkIng, _IsEngAutoSeqMkDone, _IsEngAutoSeq2dReadSt, _IsEngAutoSeqOnReading2d, _IsEngAutoSeq2dReadDone,
	// Is DlgMyMsg
	_IsMyMsgYes, _IsMyMsgNo, _IsMyMsgCancel, _IsMyMsgOk,
	// Is PLC
	_IsBuzzer, _IsTowerLamp, _IsErrorRead2dCode, _IsDispContRun,
	// EndIdx
	_EndIdx
}_SigInx;

typedef enum _stPageInx {
	_DlgHigh = 0, _Dlg01, _Dlg02, _Dlg03, _Dlg04, _Dlg05, _DlgInfo,
	_IsDlgHigh, _IsDlg01, _IsDlg02, _IsDlg03, _IsDlg04, _IsDlg05, _IsDlgInfo,
}_PageInx;

typedef enum _stItemInx {
	// Frame High
	_MkFdDiffX = 0, _MkFdDiffY, _AoiDnFdDiffX, _AoiDnFdDiffY, _AoiUpFdDiffX, _AoiUpFdDiffY, _EgrFdDiffX, _EgrFdDiffY,
	// Dlg Info	
	_SampleShotNum, _LmtTotYld, _LmtPatlYld, _StripOutRatio, _CustomNeedRatio, _NumRangeFixDef, _NumContFixDef, _UltraSonicStTim,
	// Menu01
	_OpName, _ModelUpName, _ModelDnName, _LotUpName, _LotDnName, _LayerUpName, _LayerDnName, _LoadMstInfo, _OrderNum, _ShotNum, _TotReelLen, _TotOpRto, _LotOpRto,
	_TotVel, _PartVel, _TempStopLen, _LotCutLen, _LotCutPosLen, _MkDoneLen, _AoiDnDoneLen, _AoiUpDoneLen, _LotSerial, //_MkNumLf, _MkNumRt,	
	_DefNumUp, _DefNumDn, _DefNumTot, _DefRtoUp, _DefRtoDn, _DefRtoTot, _GoodNumTot, _GoodNumUp, _GoodNumDn, _GoodRtoUp, 
	_GoodRtoDn, _GoodRtoTot, _TestNumUp, _TestNumDn, _TestNumTot, _MkVerfyLen, _LotStTime, _LotEdTime, _LotRunTime, 	
	_1LnGoodRtoUp, _1LnGoodRtoDn, _1LnGoodRtoTot, _2LnGoodRtoUp, _2LnGoodRtoDn, _2LnGoodRtoTot, _3LnGoodRtoUp, _3LnGoodRtoDn, 
	_3LnGoodRtoTot, _4LnGoodRtoUp, _4LnGoodRtoDn, _4LnGoodRtoTot, _AllLnGoodRtoUp, _AllLnGoodRtoDn, _AllLnGoodRtoTot, 	
	_DefNumOpen, _DefNumShort, _DefNumUshort, _DefNumLnW, _DefExtr, _DefNumProt, _DefNumEnick, _DefNumUdd1, _DefNumPhole, 
	_DefNumPad, _DefNumHopen, _DefNumHmiss, _DefNumHpos, _DefNumHdef, _DefNumEprot, _DefNumNrw, _DefNumLight, _DefNumNick, 
	_DefNumPoi, _DefNumVhOpen, _DefNumVhMiss, _DefNumVhPos, _DefNumVhd, _DefNumEspace, _DefNumWide,
	// Menu02
	_2DEngLen, _2DAoiLen, _2DMkLen, _2DMoveVel, _2DMoveAcc, _2DOneShotLen, _EngLeadPitch, _EngPushOffLen, _EngTqVal, _EngAoiLen,
	_EngFdDiffMax, _EngFdDiffRng, _EngFdDiffNum, _EngBuffInitPos, _EngBuffCurrPos, _EngItsCode,
	// Menu04
	_FdVel, _FdAcc, _OnePnlLen, _OnePnlVel, _OnePnlAcc, _AoiLeadPitch, _MkLeadPitch, _FdDiffMax, _FdDiffRng, _FdDiffNum, 
	_AoiPushOffLen, _MkPushOffLen, _AoiTqVal, _MkTqVal, _AoiBuffShotNum, _AoiMkLen, _MkBuffInitPos, _MkBuffCurrPos,
	_MkNumLf, _MkNumRt, _MkMaxNumLf, _MkMaxNumRt,
	_MkInitPosLf, _MkInitVelLf, _MkInitAccLf, _MkFnlPosLf, _MkFnlVelLf, _MkFnlAccLf, _MkFnlTqLf,
	_MkHgtPosX1Lf, _MkHgtPosY1Lf, _MkHgtPosX2Lf, _MkHgtPosY2Lf, _MkHgtPosX3Lf, _MkHgtPosY3Lf, _MkHgtPosX4Lf, _MkHgtPosY4Lf, _MkHgtAvgPosLf,
	_MkInitPosRt, _MkInitVelRt, _MkInitAccRt, _MkFnlPosRt, _MkFnlVelRt, _MkFnlAccRt, _MkFnlTqRt,
	_MkHgtPosX1Rt, _MkHgtPosY1Rt, _MkHgtPosX2Rt, _MkHgtPosY2Rt, _MkHgtPosX3Rt, _MkHgtPosY3Rt, _MkHgtPosX4Rt, _MkHgtPosY4Rt, _MkHgtAvgPosRt,
	// Is Frame High
	_IsMkFdDiffX, _IsMkFdDiffY, _IsAoiDnFdDiffX, _IsAoiDnFdDiffY, _IsAoiUpFdDiffX, _IsAoiUpFdDiffY, _IsEgrFdDiffX, _IsEgrFdDiffY,
	// Dlg Info	
	_IsSampleShotNum, _IsLmtTotYld, _IsLmtPatlYld, _IsStripOutRatio, _IsCustomNeedRatio, _IsNumRangeFixDef, _IsNumContFixDef, _IsUltraSonicStTim,
	// Is Menu01
	_IsOpName, _IsModelUpName, _IsModelDnName, _IsLotUpName, _IsLotDnName, _IsLayerUpName, _IsLayerDnName, _IsOrderNum, _IsShotNum, _IsTotReelLen, _IsTotOpRto, _IsLotOpRto,
	_IsTotVel, _IsPartVel, _IsTempStopLen, _IsLotCutLen, _IsLotCutPosLen, _IsMkDoneLen, _IsAoiDnDoneLen, _IsAoiUpDoneLen, _IsLotSerial, //_IsMkNumLf, _IsMkNumRt,	
	_IsDefNumUp, _IsDefNumDn, _IsDefNumTot, _IsDefRtoUp, _IsDefRtoDn, _IsDefRtoTot, _IsGoodNumTot, _IsGoodNumUp, _IsGoodNumDn, _IsGoodRtoUp,
	_IsGoodRtoDn, _IsGoodRtoTot, _IsTestNumUp, _IsTestNumDn, _IsTestNumTot, _IsMkVerfyLen, _IsLotStTime, _IsLotEdTime, _IsLotRunTime,
	_Is1LnGoodRtoUp, _Is1LnGoodRtoDn, _Is1LnGoodRtoTot, _Is2LnGoodRtoUp, _Is2LnGoodRtoDn, _Is2LnGoodRtoTot, _Is3LnGoodRtoUp, _Is3LnGoodRtoDn,
	_Is3LnGoodRtoTot, _Is4LnGoodRtoUp, _Is4LnGoodRtoDn, _Is4LnGoodRtoTot, _IsAllLnGoodRtoUp, _IsAllLnGoodRtoDn, _IsAllLnGoodRtoTot,
	_IsDefNumOpen, _IsDefNumShort, _IsDefNumUshort, _IsDefNumLnW, _IsDefExtr, _IsDefNumProt, _IsDefNumEnick, _IsDefNumUdd1, _IsDefNumPhole,
	_IsDefNumPad, _IsDefNumHopen, _IsDefNumHmiss, _IsDefNumHpos, _IsDefNumHdef, _IsDefNumEprot, _IsDefNumNrw, _IsDefNumLight, _IsDefNumNick,
	_IsDefNumPoi, _IsDefNumVhOpen, _IsDefNumVhMiss, _IsDefNumVhPos, _IsDefNumVhd, _IsDefNumEspace, _IsDefNumWide,
	// Is Menu02
	_Is2DEngLen, _Is2DAoiLen, _Is2DMkLen, _Is2DMoveVel, _Is2DMoveAcc, _Is2DOneShotLen, _IsEngLeadPitch, _IsEngPushOffLen, _IsEngTqVal, _IsEngAoiLen,
	_IsEngFdDiffMax, _IsEngFdDiffRng, _IsEngFdDiffNum, _IsEngBuffInitPos, _IsEngBuffCurrPos, _IsEngItsCode,
	// Is Menu04
	_IsFdVel, _IsFdAcc, _IsOnePnlLen, _IsOnePnlVel, _IsOnePnlAcc, _IsAoiLeadPitch, _IsMkLeadPitch, _IsFdDiffMax, _IsFdDiffRng, _IsFdDiffNum,
	_IsAoiPushOffLen, _IsMkPushOffLen, _IsAoiTqVal, _IsMkTqVal, _IsAoiBuffShotNum, _IsAoiMkLen, _IsMkBuffInitPos, _IsMkBuffCurrPos,
	_IsMkNumLf, _IsMkNumRt, _IsMkMaxNumLf, _IsMkMaxNumRt,
	_IsMkInitPosLf, _IsMkInitVelLf, _IsMkInitAccLf, _IsMkFnlPosLf, _IsMkFnlVelLf, _IsMkFnlAccLf, _IsMkFnlTqLf,
	_IsMkHgtPosX1Lf, _IsMkHgtPosY1Lf, _IsMkHgtPosX2Lf, _IsMkHgtPosY2Lf, _IsMkHgtPosX3Lf, _IsMkHgtPosY3Lf, _IsMkHgtPosX4Lf, _IsMkHgtPosY4Lf, _IsMkHgtAvgPosLf,
	_IsMkInitPosRt, _IsMkInitVelRt, _IsMkInitAccRt, _IsMkFnlPosRt, _IsMkFnlVelRt, _IsMkFnlAccRt, _IsMkFnlTqRt,
	_IsMkHgtPosX1Rt, _IsMkHgtPosY1Rt, _IsMkHgtPosX2Rt, _IsMkHgtPosY2Rt, _IsMkHgtPosX3Rt, _IsMkHgtPosY3Rt, _IsMkHgtPosX4Rt, _IsMkHgtPosY4Rt, _IsMkHgtAvgPosRt,
}_ItemInx;

typedef enum _stStatusInx {
	_StErr = 500, _StReady, _StStop, _StRun, _StInitRun, _StSglSample, _StSglTest, _StDblSample, _StDblTest,
	_IsStErr, _IsStReady, _IsStStop, _IsStRun, _IsStInitRun, _IsStSglSample, _IsStSglTest, _IsStDblSample, _IsStDblTest,
}_StatusInx;

typedef enum _stAlarmInx {
	_Alarm = 1000, _IsAlarm,
}_AlarmInx;

typedef enum _stMsgBoxInx {
	_MsgBox = 1010, _IsMsgBox,
}_MsgBoxInx;


