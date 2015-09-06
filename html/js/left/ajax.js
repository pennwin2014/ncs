function test_a(o)
{
    self.parent.mainFrame.document.body.innerHTML = "<div id='theSkendy' style='position:absolute;zIndex:10000;border:0px;text-align:center;margin-left:280px;margin-top:200px;width:214px;line-height:20px;font-size:12px;background-image: url(\"images/ing.gif\");background-repeat: no-repeat;background-position:0px 2px;font-weight:normal;'>"+o+"</div>";
}
function test_aa(o)
{
    var searchForm = document.getElementById("searchForm");
    searchForm.submit();
    document.body.innerHTML = "<div id='theSkendy' style='position:absolute;zIndex:10000;border:0px;text-align:center;margin-left:280px;margin-top:200px;width:214px;line-height:20px;font-size:12px;background-image: url(\"images/ing.gif\");background-repeat: no-repeat;background-position: 0px 2px;font-weight:normal;'>"+o+"</div>";
}
function test_aform(o,FormId)
{
    var searchForm = document.getElementById(FormId);
    searchForm.submit();
    document.body.innerHTML = "<div id='theSkendy' style='position:absolute;zIndex:10000;border:0px;text-align:center;margin-left:280px;margin-top:200px;width:214px;line-height:20px;font-size:12px;background-image: url(\"images/ing.gif\");background-repeat: no-repeat;background-position: 0px 2px;font-weight:normal;'>"+o+"</div>";
}
