var websock;
function start()
{
    websock = new WebSocket('ws://' + window.location.hostname + ':81/');
    websock.onopen = function (evt) { console.log('websock open'); };
    websock.onclose = function (evt) { console.log('websock close'); };
    websock.onerror = function (evt) { console.log(evt); };
    websock.onmessage = function (evt)
    {
        console.log(evt);
        var e = document.getElementById('ledstatus');
        if (evt.data === 'ledon')
        {
            e.style.color = 'red';
        }
        else if (evt.data === 'ledoff')
        {
            e.style.color = 'black';
        }
        else {
            console.log('unknown event');
        }
    };
}

function buttonclick(e)
{
    websock.send(e.id);
}