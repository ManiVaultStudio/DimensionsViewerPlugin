
function getRangeMark(channel, strokeWidth) {
    return {
        "mark": {
            "type": "errorband",
            //"borders": {
            //    "opacity": 0.5 * channel.opacity,
            //    "strokeWidth": strokeWidth,
            //    "strokeDash": [3, 3]
            //},
            "opacity": 0.2 * channel.opacity,
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index
                }
            }
        ],
        "encoding": {
            "x": {
                "field": "dimName",
                "type": "ordinal"
            },
            "y": {
                "field": "min",
                "type": "quantitative",
                "title": "Point value"
            },
            "y2": {
                "field": "max",
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function getAggregateLineMark(channel, strokeWidth, strokeDash) {
    return {
        "mark": {
            "type": "line",
            "strokeWidth": strokeWidth,
            "strokeDash": strokeDash,
            "strokeJoin": "round",
            "opacity": 1
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index
                }
            }
        ],
        "encoding": {
            "x": {
                "field": "dimName",
                "type": "ordinal"
            },
            "y": {
                "field": "agg",
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function getAggregatePointsMark(channel) {
    return {
        "mark": {
            "type": "point",
            "fill": channel.color,
            "opacity": 1,
            "size": 11
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index
                }
            }
        ],
        "encoding": {
            "x": {
                "field": "dimName",
                "type": "ordinal",
                "title": "Dimension"
            },
            "y": {
                "field": "agg",
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function addChannel(design, channel) {
    design.data.values = design.data.values.concat(channel.dimensions);

    if (channel.profileType > 0) {
        design.layer.push(getAggregateLineMark(channel, 2, []));
        design.layer.push(getAggregatePointsMark(channel));
    }

    if (channel.showRange)
        design.layer.push(getRangeMark(channel, 1));
}

function getDesign(configuration) {

    let design = {
        "$schema": "https://vega.github.io/schema/vega-lite/v4.json",
        "width": "container",
        "height": "container",
        "title": {
            "text": []
        },
        "config": {
            "axis": {
                "grid": true,
                "gridColor": "#dedede"
            }
        },
        "padding": 10,
        "autosize":
        {
            "type": "fit",
            "resize": true,
        },
        "signals": [],
        "data": {
            "name": "dimensions",
            "values": []
        },
        "layer": []
    }

    design.title.text.push(configuration.title);

    for (channelName in configuration.channels) {
        let channel = configuration.channels[channelName];

        addChannel(design, channel);
    }

    return design;
}