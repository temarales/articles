unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, TeEngine, Series, ExtCtrls, TeeProcs, Chart, Grids;

type
  TForm1 = class(TForm)
    Button1: TButton;
    StringGrid1: TStringGrid;
    StringGrid2: TStringGrid;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

const N=10;
      M=100;
      eps=0.1;
      
type signal=array[0..N-1] of real;
     matrix=array[0..N-1,0..N-1] of real;

function multiply(A: matrix; B: signal): signal;
var i, j: integer;
    C: signal;
begin
  for i:=0 to N-1 do
    begin
      C[i]:=0;
      for j:=0 to N-1 do
        C[i]:=C[i]+A[i][j]*B[j];
    end;
   multiply:=C;
end;

procedure TForm1.Button1Click(Sender: TObject);
var F, F1, G, index: array[0..M-1] of signal;
    i, j, k, k1, p, q, a, b, best: integer;
    alpha: signal;
    S, S1, T, T1, Sbuf, Tbuf: matrix;
    delta, delta1, buf: real;
    better: boolean;
begin
  //Генерация сигналов
  randomize();
  for p:=0 to M-1 do
    for k:=0 to N-1 do
      F[p][k]:=3*(1+random(1)/100)*exp(-sqr(k-2)/2)-
      10*(1+random(3)/100)*exp(-sqr(k-3)/3)+
      (1+random(1)/100)*exp(-sqr(k-7)/2);
  //Матрица ДПФ
  S[0][0]:=1/sqrt(2*N-1);
  for j:=1 to N-1 do
    S[0][j]:=2/sqrt(2*N-1);
  for i:=1 to N-1 do
    begin
      S[i][0]:=1/sqrt(2*N-1);
      for j:=1 to N-1 do
        S[i][j]:=2*cos(2*pi*i*j/(2*N-1))/sqrt(2*N-1);
    end;
  for i:=0 to N-1 do
    for j:=0 to N-1 do
      StringGrid1.Cells[j+1,i+1]:=FloatToStr(round(S[i][j]*1000)/1000);
  T:=S;    
  //Выбор оптимальной матрицы
  delta:=1e20;
  better:=true;
  while better do
    begin
      better:=false;
      for i:=0 to N-1 do
        begin
          best:=1;
          //Генерация преобразования по i-й строке
          for q:=1 to round( exp(N*ln(3)) ) do
            begin
              S1:=S;
              T1:=T;
              a:=q-1;
              j:=0;
              while a>0 do
                begin
                  b:=a mod 3;
                  a:=round((a-b)/3);
                  S1[i][j]:=S[i][j]+(b-1)*eps;
                  j:=j+1;
                end;
              //Нахождение обратной матрицы
              for j:=0 to N-1 do
                begin
                  alpha[j]:=0;
                  for k:=0 to N-1 do
                    alpha[j]:=alpha[j]+(S1[i][k]-S[i][k])*T[k][j];
                end;
              buf:=1+alpha[i];
              for j:=0 to N-1 do
                alpha[j]:=-alpha[j]/buf;
              for k:=0 to N-1 do
                for j:=0 to N-1 do
                  T1[k][j]:=T[k][j]+alpha[j]*T[k][i];
              //Применение преобразования к сигналам
              for p:=0 to M-1 do
                G[p]:=multiply(T1, F[p]);
              //Сжатие
              for p:=0 to M-1 do
                begin
                  for k:=0 to N-1 do
                    begin
                      index[p][k]:=1;
                      for k1:=0 to N-1 do
                        if abs(G[p][k])<abs(G[p][k1]) then
                          index[p][k]:=index[p][k]+1;
                    end;
                  for k:=0 to N-1 do
                    if index[p][k]>2 then
                      G[p][k]:=0;
                end;
              //Обратное преобразование сигналов  
              for p:=0 to M-1 do
                F1[p]:=multiply(S1, G[p]);
              //Расчет среднего отклонения
              delta1:=0;
              for p:=0 to M-1 do
                for k:=0 to N-1 do
                  delta1:=delta1+sqr(F[p][k]-F1[p][k])/(sqr(M)*sqr(N));
                delta1:=sqrt(delta1);
              if delta1<delta then
                begin
                  best:=q;
                  Sbuf:=S1;
                  Tbuf:=T1;
                  delta:=delta1;
                end;    
            end;//Конец преобразований i-й строки
          //Сохранение лучшего преобразования
          if best>1 then
            begin
              better:=true;
              S:=Sbuf;
              T:=Tbuf;
            end;
        end;//Конец перебора строк
    end;//Конец while
  for i:=0 to N-1 do
    for j:=0 to N-1 do
      StringGrid2.Cells[j+1,i+1]:=FloatToStr(round(S[i][j]*1000)/1000);
end;

procedure TForm1.FormCreate(Sender: TObject);
var i: integer;
begin
  StringGrid1.ColCount:=N+1;
  StringGrid1.RowCount:=N+1;
  StringGrid2.ColCount:=N+1;
  StringGrid2.RowCount:=N+1;
  StringGrid1.Cells[0,0]:='ДПФ';
  StringGrid2.Cells[0,0]:='S';
  for i:=1 to N do
    begin
      StringGrid1.Cells[0,i]:=FloatToStr(i);
      StringGrid1.Cells[i,0]:=FloatToStr(i);
      StringGrid2.Cells[0,i]:=FloatToStr(i);
      StringGrid2.Cells[i,0]:=FloatToStr(i);
    end;
end;

end.
