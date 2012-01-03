//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "icdagitimlist.h"
#include "icdagitim.h"
#include "datamodule.h"
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmIcDagitimList *frmIcDagitimList;
//---------------------------------------------------------------------------
__fastcall TfrmIcDagitimList::TfrmIcDagitimList(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmIcDagitimList::FormShow(TObject *Sender)
{
    // �nite Adlar� comboBox'lara dolduruluyor
    comboFirmaAdi->Clear();
    datamoduleORTEG->tblUNITE->Open();
    datamoduleORTEG->tblUNITE->First();
    comboFirmaAdi->Items->Add("t�m�...");
    while (!datamoduleORTEG->tblUNITE->Eof) {
        comboFirmaAdi->Items->Add(datamoduleORTEG->tblUNITEUNITE_ADI->Value);
        datamoduleORTEG->tblUNITE->Next();
    }
    datamoduleORTEG->tblUNITE->Close();
    // �nite Adlar� comboBox'lara dolduruldu...

    // Donem Bilgisi comboBox'a dolduruluyor...
    comboDonem->Clear();
    datamoduleORTEG->tblDONEM->Open();
    datamoduleORTEG->tblDONEM->Filtered = false;    
    datamoduleORTEG->tblDONEM->First();
    comboDonem->Items->Add("t�m�...");
    while (!datamoduleORTEG->tblDONEM->Eof) {
        comboDonem->Items->Add(datamoduleORTEG->tblDONEMDONEM_TANIMI->Value);
        datamoduleORTEG->tblDONEM->Next();
    }
    datamoduleORTEG->tblDONEM->Close();
    // Donem Bilgisi comboBox'a dolduruldu...

    // Hareket T�r� dolduruluyor...
    comboHareketTuru->Clear();
    comboHareketTuru->Items->Add("t�m�...");
    comboHareketTuru->Items->Add("IADE BELGES�");
    comboHareketTuru->Items->Add("CIKTI BELGES�");
    comboHareketTuru->Items->Add("GIRDI BELGES�");    
    comboHareketTuru->Items->Add("TEMIZLIK TUTANAGI");
    // Hareket T�r� dolduruldu...

    comboFirmaAdi->ItemIndex = 0;
    comboHareketTuru->ItemIndex = 0;
    comboDonem->ItemIndex = 0;

    comboHareketTuruChange(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmIcDagitimList::comboHareketTuruChange(TObject *Sender)
{
    AnsiString szBelgeNo, szDonemNo, szFirmaAdi, szBelgeTuru;

    if (comboHareketTuru->Text == "t�m�...")
        szBelgeTuru = "";
    else
        szBelgeTuru = comboHareketTuru->Text;

    if (editIcDagitimNo->Text.Length() > 0) {
        szBelgeNo.sprintf("%04d", StrToInt(editIcDagitimNo->Text));
    }
    else
        szBelgeNo = szBelgeNo + "%";

    if (comboDonem->Text == "t�m�...")
        szDonemNo = "";
    else
        szDonemNo = comboDonem->Text;

    if (comboFirmaAdi->Text == "t�m�...")
        szFirmaAdi = "";
    else
        szFirmaAdi = comboFirmaAdi->Text;

    queryIC_DAGITIM->Close();
    queryIC_DAGITIM->SQL->Clear();
    queryIC_DAGITIM->SQL->Add("select distinct belge_turu, belge_no, firma_adi, firma_kodu, donem from icdagitim");
    queryIC_DAGITIM->SQL->Add("where belge_turu like '%"+szBelgeTuru+"%' and belge_no like " +
        "'%"+szBelgeNo+"%' and firma_adi like '%"+szFirmaAdi + "%' and donem like '%" + szDonemNo+"%'");
    queryIC_DAGITIM->Open();
}
//---------------------------------------------------------------------------

void __fastcall TfrmIcDagitimList::comboFirmaAdiChange(TObject *Sender)
{
    comboHareketTuruChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmIcDagitimList::comboDonemChange(TObject *Sender)
{
    comboHareketTuruChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmIcDagitimList::editIcDagitimNoChange(TObject *Sender)
{
    comboHareketTuruChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmIcDagitimList::DBGrid1CellClick(TColumn *Column)
{
    int nDeger;
    TObject *Sender;

    nDeger = Application->MessageBox("Se�ti�iniz kart� onayl�yor musunuz?", "Onay Kutusu",
            MB_YESNO + MB_ICONQUESTION);

    if (nDeger == IDYES) {
        frmIcDagitim->comboDonem->ItemIndex = frmIcDagitim->comboDonem->Items->IndexOf(queryIC_DAGITIMdonem->Value);
        frmIcDagitim->comboDonemChange(Sender);
        frmIcDagitim->editBelgeNo->Text = queryIC_DAGITIMbelge_no->Value;

        if (frmIcDagitim->comboBelgeTuru->Text == "IADE BELGES�" || frmIcDagitim->comboBelgeTuru->Text == "GIRDI BELGES�")
            szBelgeTuru = "GIRDI";
        else if (frmIcDagitim->comboBelgeTuru->Text == "CIKTI BELGES�")
            szBelgeTuru = "CIKTI";
        else if (frmIcDagitim->comboBelgeTuru->Text == "TEMIZLIK TUTANAGI")
            szBelgeTuru = "TEMIZLIK";

        szBelgeNo = queryIC_DAGITIMbelge_no->Value;
        szFirmaAdi = queryIC_DAGITIMfirma_adi->Value;
        nFirmaKodu = queryIC_DAGITIMFIRMA_KODU->Value;

        frmIcDagitimList->Close();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmIcDagitimList::DBGrid1KeyPress(TObject *Sender,
      char &Key)
{
    int nDeger;

    if (Key == Char(VK_RETURN)) {
        Key = 0;
        nDeger = Application->MessageBox("Se�ti�iniz kart� onayl�yor musunuz?", "Onay Kutusu",
                MB_YESNO + MB_ICONQUESTION);

        if (nDeger == IDYES) {
            frmIcDagitim->comboDonem->ItemIndex = frmIcDagitim->comboDonem->Items->IndexOf(queryIC_DAGITIMdonem->Value);
            frmIcDagitim->comboDonemChange(Sender);
            frmIcDagitim->editBelgeNo->Text = queryIC_DAGITIMbelge_no->Value;

            if (frmIcDagitim->comboBelgeTuru->Text == "IADE BELGES�" || frmIcDagitim->comboBelgeTuru->Text == "GIRDI BELGES�")
                szBelgeTuru = "GIRDI";
            else if (frmIcDagitim->comboBelgeTuru->Text == "CIKTI BELGES�")
                szBelgeTuru = "CIKTI";
            else if (frmIcDagitim->comboBelgeTuru->Text == "TEMIZLIK TUTANAGI")
                szBelgeTuru = "TEMIZLIK";

            szBelgeNo = queryIC_DAGITIMbelge_no->Value;
            szFirmaAdi = queryIC_DAGITIMfirma_adi->Value;
            nFirmaKodu = queryIC_DAGITIMFIRMA_KODU->Value;

            frmIcDagitimList->Close();
        }
    }
}
//---------------------------------------------------------------------------

