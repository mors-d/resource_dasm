#pragma once

#include <stdint.h>

constexpr uint32_t resource_type(const char (&type)[5]) {
  return  (uint32_t(uint8_t(type[0])) << 24) |
          (uint32_t(uint8_t(type[1])) << 16) |
          (uint32_t(uint8_t(type[2])) << 8) |
          uint32_t(uint8_t(type[3]));
}
// Just to make sure the function works as intended
static_assert(resource_type("actb") == 0x61637462);

constexpr uint32_t RESOURCE_TYPE_mod  = resource_type(".mod");
// MacApp memory config for 68k; synonym of `mem!`
constexpr uint32_t RESOURCE_TYPE_68k1 = resource_type("68k!");
constexpr uint32_t RESOURCE_TYPE_actb = resource_type("actb");
constexpr uint32_t RESOURCE_TYPE_acur = resource_type("acur");
constexpr uint32_t RESOURCE_TYPE_ADBS = resource_type("ADBS");
constexpr uint32_t RESOURCE_TYPE_adio = resource_type("adio");
constexpr uint32_t RESOURCE_TYPE_AINI = resource_type("AINI");
constexpr uint32_t RESOURCE_TYPE_ALIS = resource_type("ALIS");
constexpr uint32_t RESOURCE_TYPE_alis = resource_type("alis");
constexpr uint32_t RESOURCE_TYPE_ALRT = resource_type("ALRT");
constexpr uint32_t RESOURCE_TYPE_APPL = resource_type("APPL");
constexpr uint32_t RESOURCE_TYPE_atlk = resource_type("atlk");
constexpr uint32_t RESOURCE_TYPE_audt = resource_type("audt");
constexpr uint32_t RESOURCE_TYPE_BNDL = resource_type("BNDL");
constexpr uint32_t RESOURCE_TYPE_boot = resource_type("boot");
constexpr uint32_t RESOURCE_TYPE_bstr = resource_type("bstr");
constexpr uint32_t RESOURCE_TYPE_card = resource_type("card");
constexpr uint32_t RESOURCE_TYPE_cctb = resource_type("cctb");
constexpr uint32_t RESOURCE_TYPE_CDEF = resource_type("CDEF");
constexpr uint32_t RESOURCE_TYPE_cdek = resource_type("cdek");
constexpr uint32_t RESOURCE_TYPE_cdev = resource_type("cdev");
constexpr uint32_t RESOURCE_TYPE_cfrg = resource_type("cfrg");
constexpr uint32_t RESOURCE_TYPE_cicn = resource_type("cicn");
constexpr uint32_t RESOURCE_TYPE_citt = resource_type("citt");
constexpr uint32_t RESOURCE_TYPE_clok = resource_type("clok");
constexpr uint32_t RESOURCE_TYPE_clut = resource_type("clut");
constexpr uint32_t RESOURCE_TYPE_CMDK = resource_type("CMDK");
constexpr uint32_t RESOURCE_TYPE_cmid = resource_type("cmid");
// MacApp menu definition
constexpr uint32_t RESOURCE_TYPE_CMNU = resource_type("CMNU");
// MacApp menu definition
constexpr uint32_t RESOURCE_TYPE_cmnu = resource_type("cmnu");
constexpr uint32_t RESOURCE_TYPE_cmtb = resource_type("cmtb");
constexpr uint32_t RESOURCE_TYPE_cmuN = resource_type("cmu#");
constexpr uint32_t RESOURCE_TYPE_CNTL = resource_type("CNTL");
constexpr uint32_t RESOURCE_TYPE_CODE = resource_type("CODE");
constexpr uint32_t RESOURCE_TYPE_code = resource_type("code");
constexpr uint32_t RESOURCE_TYPE_crsr = resource_type("crsr");
constexpr uint32_t RESOURCE_TYPE_csnd = resource_type("csnd");
constexpr uint32_t RESOURCE_TYPE_CTBL = resource_type("CTBL");
constexpr uint32_t RESOURCE_TYPE_CTYN = resource_type("CTY#");
constexpr uint32_t RESOURCE_TYPE_CURS = resource_type("CURS");
constexpr uint32_t RESOURCE_TYPE_dbex = resource_type("dbex");
constexpr uint32_t RESOURCE_TYPE_dcmp = resource_type("dcmp");
constexpr uint32_t RESOURCE_TYPE_dcod = resource_type("dcod");
constexpr uint32_t RESOURCE_TYPE_dctb = resource_type("dctb");
constexpr uint32_t RESOURCE_TYPE_dem  = resource_type("dem ");
constexpr uint32_t RESOURCE_TYPE_dimg = resource_type("dimg");
constexpr uint32_t RESOURCE_TYPE_DITL = resource_type("DITL");
constexpr uint32_t RESOURCE_TYPE_DLOG = resource_type("DLOG");
constexpr uint32_t RESOURCE_TYPE_DRVR = resource_type("DRVR");
constexpr uint32_t RESOURCE_TYPE_drvr = resource_type("drvr");
constexpr uint32_t RESOURCE_TYPE_ecmi = resource_type("ecmi");
constexpr uint32_t RESOURCE_TYPE_emid = resource_type("emid");
constexpr uint32_t RESOURCE_TYPE_enet = resource_type("enet");
constexpr uint32_t RESOURCE_TYPE_epch = resource_type("epch");
// MacApp error list
constexpr uint32_t RESOURCE_TYPE_errs = resource_type("errs");
constexpr uint32_t RESOURCE_TYPE_ESnd = resource_type("ESnd");
constexpr uint32_t RESOURCE_TYPE_esnd = resource_type("esnd");
constexpr uint32_t RESOURCE_TYPE_expt = resource_type("expt");
constexpr uint32_t RESOURCE_TYPE_FBTN = resource_type("FBTN");
constexpr uint32_t RESOURCE_TYPE_FCMT = resource_type("FCMT");
constexpr uint32_t RESOURCE_TYPE_fctb = resource_type("fctb");
constexpr uint32_t RESOURCE_TYPE_FDIR = resource_type("FDIR");
constexpr uint32_t RESOURCE_TYPE_finf = resource_type("finf");
constexpr uint32_t RESOURCE_TYPE_FKEY = resource_type("FKEY");
constexpr uint32_t RESOURCE_TYPE_fldN = resource_type("fld#");
constexpr uint32_t RESOURCE_TYPE_flst = resource_type("flst");
constexpr uint32_t RESOURCE_TYPE_fmap = resource_type("fmap");
constexpr uint32_t RESOURCE_TYPE_FONT = resource_type("FONT");
constexpr uint32_t RESOURCE_TYPE_fovr = resource_type("fovr");
constexpr uint32_t RESOURCE_TYPE_FREF = resource_type("FREF");
constexpr uint32_t RESOURCE_TYPE_FRSV = resource_type("FRSV");
constexpr uint32_t RESOURCE_TYPE_FWID = resource_type("FWID");
constexpr uint32_t RESOURCE_TYPE_gbly = resource_type("gbly");
constexpr uint32_t RESOURCE_TYPE_gcko = resource_type("gcko");
constexpr uint32_t RESOURCE_TYPE_GDEF = resource_type("GDEF");
constexpr uint32_t RESOURCE_TYPE_gdef = resource_type("gdef");
constexpr uint32_t RESOURCE_TYPE_gnld = resource_type("gnld");
constexpr uint32_t RESOURCE_TYPE_GNRL = resource_type("GNRL");
constexpr uint32_t RESOURCE_TYPE_gpch = resource_type("gpch");
constexpr uint32_t RESOURCE_TYPE_h8mk = resource_type("h8mk");
constexpr uint32_t RESOURCE_TYPE_hqda = resource_type("hqda");
constexpr uint32_t RESOURCE_TYPE_hwin = resource_type("hwin");
constexpr uint32_t RESOURCE_TYPE_ic04 = resource_type("ic04");
constexpr uint32_t RESOURCE_TYPE_ic05 = resource_type("ic05");
constexpr uint32_t RESOURCE_TYPE_ic07 = resource_type("ic07");
constexpr uint32_t RESOURCE_TYPE_ic08 = resource_type("ic08");
constexpr uint32_t RESOURCE_TYPE_ic09 = resource_type("ic09");
constexpr uint32_t RESOURCE_TYPE_ic10 = resource_type("ic10");
constexpr uint32_t RESOURCE_TYPE_ic11 = resource_type("ic11");
constexpr uint32_t RESOURCE_TYPE_ic12 = resource_type("ic12");
constexpr uint32_t RESOURCE_TYPE_ic13 = resource_type("ic13");
constexpr uint32_t RESOURCE_TYPE_ic14 = resource_type("ic14");
constexpr uint32_t RESOURCE_TYPE_ich4 = resource_type("ich4");
constexpr uint32_t RESOURCE_TYPE_ich8 = resource_type("ich8");
constexpr uint32_t RESOURCE_TYPE_ichN = resource_type("ich#");
constexpr uint32_t RESOURCE_TYPE_icl4 = resource_type("icl4");
constexpr uint32_t RESOURCE_TYPE_icl8 = resource_type("icl8");
constexpr uint32_t RESOURCE_TYPE_icm4 = resource_type("icm4");
constexpr uint32_t RESOURCE_TYPE_icm8 = resource_type("icm8");
constexpr uint32_t RESOURCE_TYPE_icmN = resource_type("icm#");
constexpr uint32_t RESOURCE_TYPE_icmt = resource_type("icmt");
constexpr uint32_t RESOURCE_TYPE_ICNN = resource_type("ICN#");
constexpr uint32_t RESOURCE_TYPE_icns = resource_type("icns");
constexpr uint32_t RESOURCE_TYPE_icnV = resource_type("icnV");
constexpr uint32_t RESOURCE_TYPE_ICON = resource_type("ICON");
constexpr uint32_t RESOURCE_TYPE_icp4 = resource_type("icp4");
constexpr uint32_t RESOURCE_TYPE_icp5 = resource_type("icp5");
constexpr uint32_t RESOURCE_TYPE_icp6 = resource_type("icp6");
constexpr uint32_t RESOURCE_TYPE_ics4 = resource_type("ics4");
constexpr uint32_t RESOURCE_TYPE_ics8 = resource_type("ics8");
constexpr uint32_t RESOURCE_TYPE_icsb = resource_type("icsb");
constexpr uint32_t RESOURCE_TYPE_icsB = resource_type("icsB");
constexpr uint32_t RESOURCE_TYPE_icsN = resource_type("ics#");
constexpr uint32_t RESOURCE_TYPE_ih32 = resource_type("ih32");
constexpr uint32_t RESOURCE_TYPE_il32 = resource_type("il32");
constexpr uint32_t RESOURCE_TYPE_inbb = resource_type("inbb");
constexpr uint32_t RESOURCE_TYPE_indm = resource_type("indm");
constexpr uint32_t RESOURCE_TYPE_info = resource_type("info");
constexpr uint32_t RESOURCE_TYPE_infs = resource_type("infs");
constexpr uint32_t RESOURCE_TYPE_INIT = resource_type("INIT");
constexpr uint32_t RESOURCE_TYPE_inpk = resource_type("inpk");
constexpr uint32_t RESOURCE_TYPE_inra = resource_type("inra");
constexpr uint32_t RESOURCE_TYPE_insc = resource_type("insc");
constexpr uint32_t RESOURCE_TYPE_INTL = resource_type("INTL");
constexpr uint32_t RESOURCE_TYPE_INST = resource_type("INST");
constexpr uint32_t RESOURCE_TYPE_is32 = resource_type("is32");
constexpr uint32_t RESOURCE_TYPE_it32 = resource_type("it32");
constexpr uint32_t RESOURCE_TYPE_itl0 = resource_type("itl0");
constexpr uint32_t RESOURCE_TYPE_itl1 = resource_type("itl1");
constexpr uint32_t RESOURCE_TYPE_ITL1 = resource_type("ITL1");
constexpr uint32_t RESOURCE_TYPE_itlb = resource_type("itlb");
constexpr uint32_t RESOURCE_TYPE_itlc = resource_type("itlc");
constexpr uint32_t RESOURCE_TYPE_itlk = resource_type("itlk");
constexpr uint32_t RESOURCE_TYPE_KBDN = resource_type("KBDN");
constexpr uint32_t RESOURCE_TYPE_kcs4 = resource_type("kcs4");
constexpr uint32_t RESOURCE_TYPE_kcs8 = resource_type("kcs8");
constexpr uint32_t RESOURCE_TYPE_kcsN = resource_type("kcs#");
constexpr uint32_t RESOURCE_TYPE_krnl = resource_type("krnl");
constexpr uint32_t RESOURCE_TYPE_l8mk = resource_type("l8mk");
constexpr uint32_t RESOURCE_TYPE_LAYO = resource_type("LAYO");
constexpr uint32_t RESOURCE_TYPE_LDEF = resource_type("LDEF");
constexpr uint32_t RESOURCE_TYPE_lmgr = resource_type("lmgr");
constexpr uint32_t RESOURCE_TYPE_lodr = resource_type("lodr");
constexpr uint32_t RESOURCE_TYPE_lstr = resource_type("lstr");
constexpr uint32_t RESOURCE_TYPE_ltlk = resource_type("ltlk");
constexpr uint32_t RESOURCE_TYPE_mach = resource_type("mach");
constexpr uint32_t RESOURCE_TYPE_MACS = resource_type("MACS");
constexpr uint32_t RESOURCE_TYPE_MADH = resource_type("MADH");
constexpr uint32_t RESOURCE_TYPE_MADI = resource_type("MADI");
constexpr uint32_t RESOURCE_TYPE_MBAR = resource_type("MBAR");
constexpr uint32_t RESOURCE_TYPE_MBDF = resource_type("MBDF");
constexpr uint32_t RESOURCE_TYPE_mcky = resource_type("mcky");
constexpr uint32_t RESOURCE_TYPE_MDEF = resource_type("MDEF");
// MacApp memory config
constexpr uint32_t RESOURCE_TYPE_mem1 = resource_type("mem!");
constexpr uint32_t RESOURCE_TYPE_MENU = resource_type("MENU");
constexpr uint32_t RESOURCE_TYPE_MIDI = resource_type("MIDI");
constexpr uint32_t RESOURCE_TYPE_Midi = resource_type("Midi");
constexpr uint32_t RESOURCE_TYPE_midi = resource_type("midi");
constexpr uint32_t RESOURCE_TYPE_minf = resource_type("minf");
constexpr uint32_t RESOURCE_TYPE_mitq = resource_type("mitq");
constexpr uint32_t RESOURCE_TYPE_mntr = resource_type("mntr");
constexpr uint32_t RESOURCE_TYPE_MOOV = resource_type("MOOV");
constexpr uint32_t RESOURCE_TYPE_MooV = resource_type("MooV");
constexpr uint32_t RESOURCE_TYPE_moov = resource_type("moov");
constexpr uint32_t RESOURCE_TYPE_mstr = resource_type("mstr");
constexpr uint32_t RESOURCE_TYPE_mstN = resource_type("mst#");
constexpr uint32_t RESOURCE_TYPE_name = resource_type("name");
constexpr uint32_t RESOURCE_TYPE_ncmp = resource_type("ncmp");
constexpr uint32_t RESOURCE_TYPE_ndlc = resource_type("ndlc");
constexpr uint32_t RESOURCE_TYPE_ndmc = resource_type("ndmc");
constexpr uint32_t RESOURCE_TYPE_ndrv = resource_type("ndrv");
constexpr uint32_t RESOURCE_TYPE_NFNT = resource_type("NFNT");
constexpr uint32_t RESOURCE_TYPE_nift = resource_type("nift");
constexpr uint32_t RESOURCE_TYPE_nitt = resource_type("nitt");
constexpr uint32_t RESOURCE_TYPE_nlib = resource_type("nlib");
constexpr uint32_t RESOURCE_TYPE_nrct = resource_type("nrct");
constexpr uint32_t RESOURCE_TYPE_nsnd = resource_type("nsnd");
constexpr uint32_t RESOURCE_TYPE_nsrd = resource_type("nsrd");
constexpr uint32_t RESOURCE_TYPE_ntrb = resource_type("ntrb");
constexpr uint32_t RESOURCE_TYPE_osl  = resource_type("osl ");
constexpr uint32_t RESOURCE_TYPE_otdr = resource_type("otdr");
constexpr uint32_t RESOURCE_TYPE_otlm = resource_type("otlm");
constexpr uint32_t RESOURCE_TYPE_PACK = resource_type("PACK");
constexpr uint32_t RESOURCE_TYPE_PAPA = resource_type("PAPA");
constexpr uint32_t RESOURCE_TYPE_PAT  = resource_type("PAT ");
constexpr uint32_t RESOURCE_TYPE_PATN = resource_type("PAT#");
constexpr uint32_t RESOURCE_TYPE_PICK = resource_type("PICK");
constexpr uint32_t RESOURCE_TYPE_PICT = resource_type("PICT");
constexpr uint32_t RESOURCE_TYPE_pltt = resource_type("pltt");
constexpr uint32_t RESOURCE_TYPE_pnll = resource_type("pnll");
constexpr uint32_t RESOURCE_TYPE_ppat = resource_type("ppat");
// MacApp memory config for PowerPC; synonym of `mem!`
constexpr uint32_t RESOURCE_TYPE_ppc1 = resource_type("ppc!");
constexpr uint32_t RESOURCE_TYPE_ppcc = resource_type("ppcc");
constexpr uint32_t RESOURCE_TYPE_ppci = resource_type("ppci");
constexpr uint32_t RESOURCE_TYPE_ppct = resource_type("ppct");
constexpr uint32_t RESOURCE_TYPE_PPic = resource_type("PPic");
constexpr uint32_t RESOURCE_TYPE_pptN = resource_type("ppt#");
constexpr uint32_t RESOURCE_TYPE_PRC0 = resource_type("PRC0");
constexpr uint32_t RESOURCE_TYPE_PRC3 = resource_type("PRC3");
constexpr uint32_t RESOURCE_TYPE_PREC = resource_type("PREC");
constexpr uint32_t RESOURCE_TYPE_proc = resource_type("proc");
constexpr uint32_t RESOURCE_TYPE_PSAP = resource_type("PSAP");
constexpr uint32_t RESOURCE_TYPE_pslt = resource_type("pslt");
constexpr uint32_t RESOURCE_TYPE_ptbl = resource_type("ptbl");
constexpr uint32_t RESOURCE_TYPE_PTCH = resource_type("PTCH");
constexpr uint32_t RESOURCE_TYPE_ptch = resource_type("ptch");
constexpr uint32_t RESOURCE_TYPE_pthg = resource_type("pthg");
constexpr uint32_t RESOURCE_TYPE_qrsc = resource_type("qrsc");
constexpr uint32_t RESOURCE_TYPE_qtcm = resource_type("qtcm");
// MacApp list of always resident segments; synonym of STR#
constexpr uint32_t RESOURCE_TYPE_res1 = resource_type("res!");
constexpr uint32_t RESOURCE_TYPE_RECT = resource_type("RECT");
constexpr uint32_t RESOURCE_TYPE_resf = resource_type("resf");
constexpr uint32_t RESOURCE_TYPE_RMAP = resource_type("RMAP");
constexpr uint32_t RESOURCE_TYPE_ROvN = resource_type("ROv#");
constexpr uint32_t RESOURCE_TYPE_ROvr = resource_type("ROvr");
constexpr uint32_t RESOURCE_TYPE_RSSC = resource_type("RSSC");
constexpr uint32_t RESOURCE_TYPE_rttN = resource_type("rtt#");
constexpr uint32_t RESOURCE_TYPE_RVEW = resource_type("RVEW");
constexpr uint32_t RESOURCE_TYPE_s8mk = resource_type("s8mk");
constexpr uint32_t RESOURCE_TYPE_sb24 = resource_type("sb24");
constexpr uint32_t RESOURCE_TYPE_SB24 = resource_type("SB24");
constexpr uint32_t RESOURCE_TYPE_sbtp = resource_type("sbtp");
constexpr uint32_t RESOURCE_TYPE_scal = resource_type("scal");
constexpr uint32_t RESOURCE_TYPE_scod = resource_type("scod");
constexpr uint32_t RESOURCE_TYPE_scrn = resource_type("scrn");
constexpr uint32_t RESOURCE_TYPE_sect = resource_type("sect");
// MacApp list of segments; synonym of STR#
constexpr uint32_t RESOURCE_TYPE_seg1 = resource_type("seg!");
constexpr uint32_t RESOURCE_TYPE_SERD = resource_type("SERD");
constexpr uint32_t RESOURCE_TYPE_sfnt = resource_type("sfnt");
constexpr uint32_t RESOURCE_TYPE_sfvr = resource_type("sfvr");
constexpr uint32_t RESOURCE_TYPE_shal = resource_type("shal");
constexpr uint32_t RESOURCE_TYPE_SICN = resource_type("SICN");
constexpr uint32_t RESOURCE_TYPE_sift = resource_type("sift");
constexpr uint32_t RESOURCE_TYPE_SIGN = resource_type("SIGN");
constexpr uint32_t RESOURCE_TYPE_SIZE = resource_type("SIZE");
constexpr uint32_t RESOURCE_TYPE_slct = resource_type("slct");
constexpr uint32_t RESOURCE_TYPE_slut = resource_type("slut");
constexpr uint32_t RESOURCE_TYPE_SMOD = resource_type("SMOD");
constexpr uint32_t RESOURCE_TYPE_SMSD = resource_type("SMSD");
constexpr uint32_t RESOURCE_TYPE_snd  = resource_type("snd ");
constexpr uint32_t RESOURCE_TYPE_snth = resource_type("snth");
constexpr uint32_t RESOURCE_TYPE_SONG = resource_type("SONG");
constexpr uint32_t RESOURCE_TYPE_SOUN = resource_type("SOUN");
constexpr uint32_t RESOURCE_TYPE_STR  = resource_type("STR ");
constexpr uint32_t RESOURCE_TYPE_STRN = resource_type("STR#");
constexpr uint32_t RESOURCE_TYPE_styl = resource_type("styl");
constexpr uint32_t RESOURCE_TYPE_t8mk = resource_type("t8mk");
constexpr uint32_t RESOURCE_TYPE_tdig = resource_type("tdig");
constexpr uint32_t RESOURCE_TYPE_TEXT = resource_type("TEXT");
constexpr uint32_t RESOURCE_TYPE_thnN = resource_type("thn#");
constexpr uint32_t RESOURCE_TYPE_TMPL = resource_type("TMPL");
constexpr uint32_t RESOURCE_TYPE_TOC  = resource_type("TOC ");
constexpr uint32_t RESOURCE_TYPE_tokn = resource_type("tokn");
constexpr uint32_t RESOURCE_TYPE_TOOL = resource_type("TOOL");
constexpr uint32_t RESOURCE_TYPE_Tune = resource_type("Tune");
// MacApp Text Style
constexpr uint32_t RESOURCE_TYPE_TxSt = resource_type("TxSt");
constexpr uint32_t RESOURCE_TYPE_vdig = resource_type("vdig");
constexpr uint32_t RESOURCE_TYPE_vers = resource_type("vers");
constexpr uint32_t RESOURCE_TYPE_wart = resource_type("wart");
constexpr uint32_t RESOURCE_TYPE_wctb = resource_type("wctb");
constexpr uint32_t RESOURCE_TYPE_WDEF = resource_type("WDEF");
constexpr uint32_t RESOURCE_TYPE_WIND = resource_type("WIND");
constexpr uint32_t RESOURCE_TYPE_wstr = resource_type("wstr");
constexpr uint32_t RESOURCE_TYPE_XCMD = resource_type("XCMD");
constexpr uint32_t RESOURCE_TYPE_XFCN = resource_type("XFCN");
constexpr uint32_t RESOURCE_TYPE_Ysnd = resource_type("Ysnd");
