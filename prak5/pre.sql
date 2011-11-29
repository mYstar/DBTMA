p_rename Mitarbeiter Mitarbeiter_old

create table Mitgeh(
  Mitnr char(5),
  Gehgr char(3)
)

create table Gehaelter(
  Gehalt float,
  Gehgr char(3)
)

sp_addtabledef "Gehaelter", "syb125c.ma11s67766.dbo.Gehaelter"
sp_addtabledef "Mitgeh",  "syb125c.ma11s67766.dbo.Mitgeh"

create existing table Mitgeh(
  Mitnr char(5),
  Gehgr char(3)
) at "syb125c.ma11s67766.dbo.Mitgeh"

create existing table gehaelter(
  Gehalt float,
  Gehgr char(3)
) at "syb125c.ma11s67766.dbo.gehaelter"

insert into Mitgeh select * from depot.dbo.quellemitgeh
insert into gehaelter select * from depot.dbo.Gehaelter

alter table Mitarbeiter add primary key(Mitnr)
alter table gehaelter add primary key(Gehgr)
alter table Mitgeh add primary key(Gehgr, Mitnr)

-- 5.1
select m.Name, m.Ort, g.Gehalt
from Mitarbeiter m, gehaelter g, Mitgeh mg
where m.Name = "Fuchs" and
    m.Mitnr = mg.Mitnr and
    mg.Gehgr = g.Gehgr

-- 5.2
select AVG(g.Gehalt)
from Mitarbeiter m, gehaelter g, Mitgeh mg
where m.Ort = "Dresden" and
    m.Mitnr = mg.Mitnr and
    mg.Gehgr = g.Gehgr

-- 5.3
select m.Name, g.Gehalt
from Mitarbeiter m, gehaelter g, Mitgeh mg, (select MAX(g.Gehalt) AS maxi from gehaelter g) UA
where g.Gehalt = UA.maxi and
    m.Mitnr = mg.Mitnr and
    mg.Gehgr = g.Gehgr

-- 5.4
select m.Name, g.Gehalt
from Mitarbeiter m, gehaelter g, Mitgeh mg, (select MIN(g.Gehalt) AS mini from gehaelter g) UA
where g.Gehalt = UA.mini and
    m.Mitnr = mg.Mitnr and
    mg.Gehgr = g.Gehgr

-- 5.5
update Mitgeh set Gehgr = (
    select MAX(mg.Gehgr)
    from Mitgeh mg, (select mg.Gehgr as old_geh
        from Mitarbeiter m, Mitgeh mg
        where m.Mitnr = mg.Mitnr and
            m.Name = "Elster") UA
        where mg.Gehgr < UA.old_geh
    )
where Mitgeh.Mitnr = (Select Mitnr from Mitarbeiter m where m.Name = "Elster")

-- 5.6
create trigger t_ins_mit
on Mitgeh
for insert
as
begin
    if ((select Count(*) from Mitgeh mg where mg.Mitnr NOT IN(select Mitnr from Mitarbeiter)) <> 0)
    begin
        print "create Mitarbeiter first"
        rollback transaction
    end
    if ((select Count(*) from Mitgeh mg where mg.Gehgr NOT IN(select Gehgr from gehaelter)) <> 0)
    begin
        print "create Gehgr first"
        rollback transaction
    end
end

--5.7
create trigger t_del_mit
on Mitarbeiter
for delete
as
begin
  delete Mitgeh where Mitnr NOT IN (select Mitnr from Mitarbeiter)
end
